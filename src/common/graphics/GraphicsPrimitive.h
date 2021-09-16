//
// Created by thallock on 9/5/21.
//

#ifndef GLVIEW_GRAPHICSPRIMITIVE_H
#define GLVIEW_GRAPHICSPRIMITIVE_H

#include "../utils/Serializable.h"

namespace billiards::gphx {
	
	class GraphicsPrimitive : public json::Serializable {
	public:
		int priority;

		GraphicsPrimitive() : priority{10} {};
		~GraphicsPrimitive() override = default;

//		virtual void render(BilliardsView *render) = 0;

		[[nodiscard]] virtual std::string get_type() const = 0;

		inline
		void to_json(json::SaxWriter& writer) const override {
			writer.field("type", get_type());
		}
	};

}


#endif //GLVIEW_GRAPHICSPRIMITIVE_H
