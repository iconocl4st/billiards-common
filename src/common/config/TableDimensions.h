//
// Created by thallock on 9/10/21.
//

#ifndef GLVIEW_TABLEDIMENSIONS_H
#define GLVIEW_TABLEDIMENSIONS_H

namespace billiards::config {

	class TableDimensions {
	public:
		double width;
		double height;

		TableDimensions(double w, double h) : width{w}, height{h} {}
		TableDimensions() : width{92}, height{46} {}

		~TableDimensions() = default;
	};

}


#endif //GLVIEW_TABLEDIMENSIONS_H
