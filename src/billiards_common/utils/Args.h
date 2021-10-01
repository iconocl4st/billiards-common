//
// Created by thallock on 9/16/21.
//

#ifndef GLVIEW_ARGS_H
#define GLVIEW_ARGS_H

namespace billiards::utils {

	class Args {
	public:
		int argc;
		char **argv;

		Args(int argc, char **argv) : argc{argc}, argv{argv} {}
		virtual ~Args() = default;
	};

}


#endif //GLVIEW_ARGS_H
