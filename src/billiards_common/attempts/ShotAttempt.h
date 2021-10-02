//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTATTEMPT_H
#define GLVIEW_SHOTATTEMPT_H

#include "billiards_common/shots/Shot.h"
#include "billiards_common/shots/Positions.h"
#include "billiards_common/attempts/AttemptResult.h"

namespace billiards::attempts {

	class ShotAttempt : public json::Serializable {
	public:
		layout::Layout layout;
		shots::Shot shot;
		uint64_t date;
		AttemptResult result;

		ShotAttempt() = default;
		virtual ~ShotAttempt() = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("shot");
			writer.
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			// TODO
		}
	};

}


#endif //GLVIEW_SHOTATTEMPT_H
