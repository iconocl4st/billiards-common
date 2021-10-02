//
// Created by thallock on 9/20/21.
//

#ifndef IDEA_GRAPHICSRECEIVER_H
#define IDEA_GRAPHICSRECEIVER_H

#include "billiards_common/graphics/Lines.h"
#include "billiards_common/graphics/Polygon.h"
#include "billiards_common/graphics/Circle.h"
#include "billiards_common/graphics/Text.h"
#include "billiards_common/graphics/Image.h"

namespace billiards::graphics {
	class GraphicsReceiver {
	public:
		GraphicsReceiver() = default;
		virtual ~GraphicsReceiver() = default;

		virtual void fill(const Circle *graphics) const = 0;
		virtual void fill(const Polygon *graphics) const = 0;
//		virtual void fill(const Lines *graphics) const = 0;

		virtual void draw(const Circle *graphics) const = 0;
		virtual void draw(const Polygon *graphics) const = 0;
		virtual void draw(const Lines *graphics) const = 0;

		virtual void draw(const Text *graphics) const = 0;
		virtual void draw(const Image *graphics) const = 0;

		void accept(const GraphicsPrimitive *ptr) const {
			if (ptr == nullptr) {
				return;
			}
			std::string type = ptr->get_type();
			if (type == "text") {
				draw((const Text *) ptr);
			} else if (type == "image") {
				draw((const Image *) ptr);
			} else if (type == "lines") {
				draw((const Lines *) ptr);
			} else if (type == "circle") {
				const auto *s = (const Circle *) ptr;
				if (s->fill) {
					fill(s);
				} else {
					draw(s);
				}
			} else if (type == "polygon") {
				const auto *s = (const Polygon *) ptr;
				if (s->fill) {
					fill(s);
				} else {
					draw(s);
				}
			}
		}
	};
}

#endif //IDEA_GRAPHICSRECEIVER_H
