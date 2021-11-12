//
// Created by thallock on 11/11/21.
//

#ifndef IDEA_RECORD_INFO
#define IDEA_RECORD_INFO

#include <ctime>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/lexical_cast.hpp>

#include "billiards_common/utils/Serializable.h"

namespace billiards::unql {

	class RecordInfo : public json::Serializable {
	public:
		boost::uuids::uuid uuid;
		uint64_t creation_time;
		uint64_t modification_time;

//		RecordInfo(boost::uuids::uuid uuid, uint64_t ctime, uint64_t mtime)
//				: uuid{uuid}
//				, creation_time{ctime}
//				, modification_time{mtime}
//		{}

//		explicit RecordInfo(boost::uuids::uuid uuid)
//			: RecordInfo{uuid, std::time(nullptr), std::time(nullptr)} {}

		RecordInfo(): uuid{}, creation_time{0}, modification_time{0} {}

		~RecordInfo() override = default;

		bool operator<(const RecordInfo& entry) const {
			return uuid < entry.uuid;
		}

		RecordInfo& operator=(const RecordInfo& other) {
			uuid = other.uuid;
			creation_time = other.creation_time;
			modification_time = other.modification_time;
			return *this;
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_STRING(status, value, "uuid", "Entry must have a uuid");
			boost::uuids::string_generator gen;
			uuid = gen(value["uuid"].get<std::string>());
			ENSURE_NUMBER(status, value, "creation-time", "Entry must have a creation date.");
			creation_time = (std::time_t) value["creation-time"].get<uint64_t>();
			ENSURE_NUMBER(status, value, "modification-time", "Entry must have a modification date.");
			modification_time = (std::time_t) value["modification-time"].get<uint64_t>();
			// modification_time = value["last-modified"].get<long>();
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.string_field("uuid", boost::uuids::to_string(uuid));
			writer.field("creation-time", creation_time);
			writer.field("modification-time", modification_time);
			writer.end_object();
		}
	};
}


#endif // IDEA_RECORD_INFO
