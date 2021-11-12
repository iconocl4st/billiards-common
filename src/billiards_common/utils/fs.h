//
// Created by thallock on 11/7/21.
//

#ifndef IDEA_FS_H
#define IDEA_FS_H


#include <iostream>
#include <boost/filesystem.hpp>

namespace billiards::utils {

    bool ensure_directory(const std::string& path) {
        boost::filesystem::path dir{path};
        if (!boost::filesystem::exists(dir)) {
            boost::filesystem::create_directories(dir);
        }
        return boost::filesystem::is_directory(dir);
    }

    void list_directory(const std::string& path, const std::function<void(const std::string&)>& rec) {
        if (!ensure_directory(path)) {
            return;
        }
        boost::filesystem::path dir{path};
        for (auto& child : boost::filesystem::directory_iterator(dir)) {
            rec(child.path().string());
        }
    }

}
#endif //IDEA_FS_H
