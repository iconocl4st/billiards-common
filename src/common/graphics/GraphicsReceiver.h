//
// Created by thallock on 9/20/21.
//

#ifndef IDEA_GRAPHICSRECEIVER_H
#define IDEA_GRAPHICSRECEIVER_H

#include "Lines.h"
#include "Polygon.h"
#include "Circle.h"

namespace billiards::gphx {
	class GraphicsReceiver {
	public:
		GraphicsReceiver() = default;
		virtual ~GraphicsReceiver() = default;

		virtual void receive(const Circle *graphics) const = 0;
		virtual void receive(const Lines *graphics) const = 0;
		virtual void receive(const Polygon *graphics) const = 0;

		void accept(const GraphicsPrimitive *ptr) const {
			if (ptr == nullptr) {
				return;
			}
			std::string type = ptr->get_type();
			if (type == "circle") {
				receive((const Circle *) ptr);
			} else if (type == "lines") {
				receive((const Lines *) ptr);
			} else if (type == "polygon") {
				receive((const Polygon *) ptr);
			}
		}
	};
}

#endif //IDEA_GRAPHICSRECEIVER_H
