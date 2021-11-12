//
// Created by thallock on 11/11/21.
//

#ifndef IDEA_CONNECTION_H
#define IDEA_CONNECTION_H

//#include <utility>

#include <unqlite.h>

#include "billiards_common/utils/dump.h"
#include "billiards_common/unql/Buffer.h"

namespace billiards::unql {

	class DbConnection {
	public:
		unqlite *db;
		int rc;
		bool initialized;

		DbConnection(const std::string& file, int flags)
				: db{nullptr}
				, rc{unqlite_open(&db, file.c_str(), flags)}
				, initialized{rc == UNQLITE_OK}
		{}

		virtual ~DbConnection() {
			if (!initialized) {
				return;
			}
			unqlite_close(db);
		}

		[[nodiscard]] inline
		bool has_error() const {
			if (rc == UNQLITE_OK) {
				return false;
			}

			const char *zBuf;
			int iLen;

			/* Something goes wrong, extract database error log */
			unqlite_config(db, UNQLITE_CONFIG_ERR_LOG, &zBuf, &iLen);
			if (iLen > 0) {
				std::cout << zBuf << std::endl;
			}

			if (rc != UNQLITE_BUSY && rc != UNQLITE_NOTIMPLEMENTED) {
				/* Rollback */
				unqlite_rollback(db);
			}

			return true;
		}

		template <class C>
		bool fetch(const std::string& key, Buffer buff, C& c) {
			unqlite_int64 nb = 0;
			rc = unqlite_kv_fetch(db, key.c_str(), key.size(), nullptr, &nb);
			if (rc == UNQLITE_NOTFOUND) {
				return false;
			}
			if (has_error()) {
				return false;
			}
			buff.ensure(nb + 1);

			rc = unqlite_kv_fetch(db, key.c_str(), key.size(), buff.buff, &nb);
			if (rc == UNQLITE_NOTFOUND) {
				return false;
			}
			if (has_error()) {
				return false;
			}
			buff.buff[nb] = 0;

			auto value = nlohmann::json::parse(buff.buff);
			billiards::json::ParseResult result;
			c.parse(value, result);
			if (!result.success) {
				std::cerr << "Error parsing from db\n";
				std::cerr << result.error_msg.str() << std::endl;
				return false;
			}

			return true;
		}

		template <class C>
		bool fetch(unqlite_kv_cursor *cursor, Buffer buff, C& c) {
			int key_size = 0;
			rc = unqlite_kv_cursor_key(cursor, nullptr, &key_size);
			if (has_error()) {
				return false;
			}
			buff.ensure(key_size + 1);

			rc = unqlite_kv_cursor_key(cursor, buff.buff, &key_size);
			if (has_error()) {
				return false;
			}
			buff.buff[key_size] = 0;

			boost::uuids::uuid uuid{};

			try {
				boost::uuids::string_generator gen;
				uuid = gen(buff.buff);
			} catch (std::exception& ex) {
				std::cerr << "Unable to parse uuid from cursor:" << std::endl;
				std::cerr << ex.what() << std::endl;
				return false;
			} catch (...) {
				std::cerr << "Unable to parse uuid from cursor, unknown cause." << std::endl;
				return false;
			}

			unqlite_int64 data_size = 0;
			rc = unqlite_kv_cursor_data(cursor, nullptr, &data_size);
			if (has_error()) {
				return false;
			}
			buff.ensure(data_size + 1);

			rc = unqlite_kv_cursor_data(cursor, buff.buff, &data_size);
			if (has_error()) {
				return false;
			}

			buff.buff[data_size] = 0;
			auto value = nlohmann::json::parse(buff.buff);
			billiards::json::ParseResult result;
			c.parse(value, result);
			if (!result.success) {
				std::cerr << "Error parsing from db\n";
				std::cerr << result.error_msg.str() << std::endl;
				return false;
			}

			return true;
		}


		template <class C>
		inline bool store(const std::string& key, const C& c) {
			auto str = json::dump(c);
			rc = unqlite_kv_store(db, key.c_str(), key.size(), str.c_str(), str.size());
			return !has_error();
		}
	};
}

#endif //IDEA_CONNECTION_H
