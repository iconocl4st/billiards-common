//
// Created by thallock on 11/11/21.
//

#ifndef IDEA_LAYOUTLISTITEM_H
#define IDEA_LAYOUTLISTITEM_H

/*
 * The same as a layout, but only the information used for listing.
 */
namespace billiards::layout {

	class LayoutListItem : public billiards::json::Serializable {
	public:
		std::string name;

		LayoutListItem() : name{"Name to be filled in"} {}

		~LayoutListItem() override = default;

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			ENSURE_STRING(status, value, "name", "Must have a name");
			name = value["name"].get<std::string>();
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.field("name", name);
			writer.end_object();
		}
	};
}

#endif //IDEA_LAYOUTLISTITEM_H
