//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTATTEMPT_H
#define GLVIEW_SHOTATTEMPT_H

#include "./Shot.h"

namespace billiards::shots {

	class ShotAttempt : public json::Serializable {
	public:
		Shot shot;
		uint64_t date;
		bool success;

		ShotAttempt() = default;
		virtual ~ShotAttempt() = default;

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.end_object();
		}

		void parse(const nlohmann::json& value) override {
			// TODO
		}
	};

}


#endif //GLVIEW_SHOTATTEMPT_H
