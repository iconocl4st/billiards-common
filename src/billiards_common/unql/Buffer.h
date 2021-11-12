//
// Created by thallock on 11/11/21.
//

#ifndef IDEA_BUFFER_H
#define IDEA_BUFFER_H

namespace billiards::unql {

	class Buffer {
	public:
		size_t size;
		char *buff;

		Buffer() : size{0}, buff{nullptr} {}

		explicit Buffer(size_t size) : size{size}, buff{new char[size]} {}

		inline
		virtual ~Buffer() {
			delete[] buff;
		}

		inline
		void ensure(size_t nb) {
			if (nb <= size) {
				return;
			}
			delete[] buff;
			buff = new char[nb];
			size = nb;
		}
	};
}

#endif //IDEA_BUFFER_H
