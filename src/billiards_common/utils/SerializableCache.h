//
// Created by thallock on 10/27/21.
//

#ifndef IDEA_SERIALIZABLECACHE_H
#define IDEA_SERIALIZABLECACHE_H

#include <fstream>
#include <iomanip>

#include "billiards_common/utils/Serializable.h"
#include "billiards_common/utils/NlohmannWriter.h"

namespace billiards::json {

    template<class S>
    class SerializableCache {
    public:
        S cached;
        std::string path;

        explicit SerializableCache(std::string path)
            : cached{}
            , path{std::move(path)}
        {}

        virtual ~SerializableCache() = default;

        inline
        bool reload() {
            bool loaded = false;
            try {
                std::ifstream ifs{path};
                auto value = nlohmann::json::parse(ifs);
                billiards::json::ParseResult result;
                cached.parse(value, result);
                if (result.success) {
                    loaded = true;
                } else {
                    std::cerr << "Error reading " << path << std::endl;
                    std::cerr << result.error_msg.str() << std::endl;
                }
            } catch (const std::exception& ex) {
                std::cerr << "Exception reading json from " << path << std::endl;
                std::cerr << ex.what() << std::endl;
            } catch(...) {
                std::cerr << "Unknown error reading json from " << path << std::endl;
            }
            if (!loaded) {
                save();
            }
            return loaded;
        }

        inline
        bool save() const {
            std::cout << "Writing to " << path << std::endl;
            NlohmannWriter writer;
            SaxWriter& w{writer};
            cached.to_json(w);
            std::ofstream ofs{path};
            ofs << std::setw(4) << writer.get_root() << "\n";
            return true;
        }
    };
}

#endif //IDEA_SERIALIZABLECACHE_H
