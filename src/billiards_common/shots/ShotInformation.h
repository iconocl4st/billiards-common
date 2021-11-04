//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_SHOTINFORMATION_H
#define GLVIEW_SHOTINFORMATION_H

#include "billiards_common/shots/Shot.h"
#include "billiards_common/shots/CueingInfo.h"

namespace billiards::shots {

	class ShotInformation : public json::Serializable {
	public:
		Shot shot;
		CueingInfo cueing;
		std::vector<StepInfo> infos;
		bool valid;

		explicit ShotInformation(const Shot& shot)
			: shot{shot}
			, cueing{}
			, infos{}
			, valid{true}
		{}
		ShotInformation()
			: shot{}
			, cueing{}
			, infos{}
			, valid{true}
		{}

		~ShotInformation() override = default;

		inline
		void invalidate() {
            valid = false;
		}

		inline
		void update_validity(bool b) {
            if (!valid) {
                invalidate();
            }
		}

		[[nodiscard]] inline
		std::shared_ptr<ShotStep>& get_step(const StepInfo& dest) {
			const int step_index = dest.step_index;
			if (step_index < 0 || step_index >= shot.steps.size()) {
				throw std::runtime_error{"Invalid index"};
			}
			return shot.steps[step_index];
		}

		[[nodiscard]] inline
		const std::shared_ptr<ShotStep>& get_step(const StepInfo& dest) const {
			const int step_index = dest.step_index;
			if (step_index < 0 || step_index >= shot.steps.size()) {
				throw std::runtime_error{"Invalid index"};
			}
			return shot.steps[step_index];
		}

		template<class StepType>
		std::shared_ptr<StepType> get_typed_step(const StepInfo& dest) const {
			return std::dynamic_pointer_cast<StepType>(get_step(dest));
		}

		[[nodiscard]] inline
		step_type::ShotStepType get_shot_type(const StepInfo& dest) const {
			return get_step(dest)->type;
		}

		[[nodiscard]] inline
		step_type::ShotStepType get_shot_type(int dest_index) const {
			if (dest_index < 0 || dest_index >= infos.size()) {
				throw std::runtime_error{"Invalid destination index"};
			}
			return get_shot_type(infos[dest_index]);
		}

		[[nodiscard]] inline
		const StepInfo& get_info(int dest_index) const {
			if (dest_index < 0 || dest_index >= infos.size()) {
				throw std::runtime_error{"Invalid destination index"};
			}
			return infos[dest_index];
		}

		[[nodiscard]] inline
		StepInfo& get_info(int dest_index) {
			if (dest_index < 0 || dest_index >= infos.size()) {
				throw std::runtime_error{"Invalid destination index"};
			}
			return infos[dest_index];
		}

		void to_json(json::SaxWriter& writer) const override {
			writer.begin_object();
			writer.key("shot");
			shot.to_json(writer);
			writer.key("cueing");
			cueing.to_json(writer);
			writer.key("infos");
			writer.begin_array();
			for (const auto& dest : infos) {
				dest.to_json(writer);
			}
			writer.end_array();
			writer.end_object();
		}

		void parse(const nlohmann::json& value, json::ParseResult& status) override {
			REQUIRE_CHILD(status, value, "shot", shot, "Shot info must have a shot");
			REQUIRE_CHILD(status, value, "cueing", cueing, "Shot info must have a cueing info");
			ENSURE_ARRAY(status, value, "infos", "Shot info must have destinations");
			infos.clear();
			for (const auto& it: value["infos"]) {
				infos.emplace_back();
				PARSE_CHILD(status, it, infos.back());
			}
		}
	};


}


#endif //GLVIEW_SHOTINFORMATION_H
