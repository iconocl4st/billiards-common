
//
// Created by thallock on 11/10/21.
//

#ifndef IDEA_RECORD_H
#define IDEA_RECORD_H

#include "billiards_common/unql/RecordInfo.h"

namespace billiards::unql {

	template <class C>
	class Record : public billiards::json::Serializable {
	public:
		RecordInfo info;
		C obj;

		Record() : info{}, obj{} {}

//		explicit Record(const boost::uuids::uuid& uuid) : info{uuid}, obj{} {}

		~Record() override = default;

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "info", info, "Must have a record info");
			REQUIRE_CHILD(status, value, "entry", obj, "Must have an entry");
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();

			writer.key("info");
			info.to_json(writer);

			writer.key("entry");
			obj.to_json(writer);

			writer.end_object();
		}
	};
}

#endif //IDEA_RECORD_H
