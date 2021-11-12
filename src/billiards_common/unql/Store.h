//
// Created by thallock on 11/11/21.
//

#ifndef IDEA_STORE_H
#define IDEA_STORE_H

#include <functional>
#include <set>
#include <boost/uuid/uuid_generators.hpp>

#include "billiards_common/utils/dump.h"
#include "billiards_common/unql/Connection.h"
#include "billiards_common/unql/Record.h"
#include "billiards_common/utils/current_time.h"


namespace billiards::unql {

	class Cursor {
	public:
		DbConnection& con;
		unqlite_kv_cursor *cursor;
		int rc;
		bool initialized;

		explicit Cursor(DbConnection& c)
			: con{c}
			, cursor{nullptr}
			, rc{unqlite_kv_cursor_init(con.db, &cursor)}
			, initialized{rc == UNQLITE_OK}
		{}

		~Cursor()  {
			if (!initialized) {
				return;
			}

			unqlite_kv_cursor_release(con.db, cursor);
		}
	};

	template <class C>
	class UnqlStore {
	public:
		std::string db_file;
		boost::uuids::random_generator gen;

		explicit UnqlStore(std::string path)
			: db_file{std::move(path)}
			, gen{}
		{}

		virtual ~UnqlStore() = default;

		bool clear() {
			int N = 50;

			while (true) {
				std::set<boost::uuids::uuid> to_remove;
				std::function<bool(const Record<C>&)> receiver = [&](const Record<C> &r) {
					to_remove.insert(r.info.uuid);
					return to_remove.size() < N;
				};
				if (!list(receiver)) {
					return false;
				}
				if (to_remove.empty()) {
					return true;
				}
				for (const auto& uuid : to_remove) {
					if (!remove(uuid)) {
						return false;
					}
				}
			}
		}

		bool list(std::function<bool(const Record<C>&)>& receiver) const {
			DbConnection con{db_file, UNQLITE_OPEN_CREATE};
			if (con.has_error()) {
				return false;
			}

			Cursor cursor{con};
			con.rc = cursor.rc;
			if (con.has_error()) {
				return false;
			}

			Buffer buff;
			for (unqlite_kv_cursor_first_entry(cursor.cursor); unqlite_kv_cursor_valid_entry(cursor.cursor); unqlite_kv_cursor_next_entry(cursor.cursor)) {
				Record<C> record;
				bool success = con.fetch(cursor.cursor, buff, record);
				if (!success) {
					return false;
				}
				bool cont = receiver(record);
				if (!cont) {
					break;
				}
			}

			return true;
		}

		bool fetch(const boost::uuids::uuid& uuid, Record<C>& record) const {
			DbConnection con{db_file, UNQLITE_OPEN_READONLY | UNQLITE_OPEN_MMAP};
			if (con.has_error()) {
				return false;
			}
			Buffer buffer;
			auto key = boost::uuids::to_string(uuid);
			bool success = con.fetch<Record<C>>(key, buffer, record);
			return success;
		}

		bool create(Record<C>& result) {
			DbConnection con{db_file, UNQLITE_OPEN_MMAP};
			if (con.has_error()) {
				return false;
			}

			std::string new_key;
			while (true) {
				result.info.uuid = gen();
				new_key = boost::uuids::to_string(result.info.uuid);

				unqlite_int64 tmp = 0;
				con.rc = unqlite_kv_fetch(con.db, new_key.c_str(), new_key.size(), nullptr, &tmp);
				if (con.rc == UNQLITE_NOTFOUND) {
					break;
				}
				if (con.has_error()) {
					return false;
				}
			}
			result.info.creation_time = billiards::utils::now();
			result.info.modification_time = result.info.creation_time;

			auto key = boost::uuids::to_string(result.info.uuid);
			bool success = con.store<Record<C>>(key, result);
			return success;
		}

		bool remove(const boost::uuids::uuid& uuid) const {
			DbConnection con{db_file, UNQLITE_OPEN_MMAP};
			if (con.has_error()) {
				return false;
			}

			auto key = boost::uuids::to_string(uuid);
			con.rc = unqlite_kv_delete(con.db, key.c_str(), key.size());
			return !con.has_error();
		}

		bool update(const boost::uuids::uuid& uuid, const nlohmann::json& updates, Record<C>& record) const {
			DbConnection con{db_file, UNQLITE_OPEN_MMAP};
			if (con.has_error()) {
				return false;
			}

			Buffer buffer;

			auto key = boost::uuids::to_string(uuid);
			bool success = con.fetch<Record<C>>(key, buffer, record);
			if (!success) {
				return false;
			}

			json::ParseResult status;
			record.obj.parse(updates, status);
			if (!status.success) {
				// TODO
				return false;
			}
			record.info.modification_time = billiards::utils::now();
			con.store<Record<C>>(key, record);
			if (con.has_error()) {
				return false;
			}
			return true;
		}
/*
		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_STRING(status, value, "directory", "Manager must have a directory");
			uuid = boost::lexical_cast<boost::uuids::uuid>(value["directory"].get<std::string>());
			ENSURE_ARRAY(status, value, "entries", "Manager must have entries.");
			for (const auto& e : value["entries"]) {
				LayoutEntry entry;
				entry.parse(e, status);
				if (!status.success) {
					return;
				}

				entries.insert(entry);
			}
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("directory", directory);
			writer.key("entries");
			writer.begin_array();
			for (const auto& entry : entries) {
				entry.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}
		*/
	};
}

#endif //IDEA_STORE_H
