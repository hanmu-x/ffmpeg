
#include "tool_dir_name/project.h"
#include "tool_dir_name/config.hpp"

#include <filesystem>






int main() 
{
    const char* outputFileName = "output_video.raw";
    int width = 640;
    int height = 480;
    tool_class tc;

    tc.pullVideoFromCamera(outputFileName, width, height);
    return 0;
}






//int main()
//{
//
//	Config config;
//#ifndef NDEBUG
//	std::string configPath = "../../../../Config/my_config.json";
//#else
//	std::string configPath = "./my_config.json";
//#endif
//    if (config.read_config(configPath))
//    {
//        std::cout << "Read config file succession " << std::endl;
//    }
//    else
//    {
//        std::cout << "ERROR : Failed to read config file " << std::endl;
//        return 1;
//    }
//
//    std::filesystem::path data_1(DEFAULT_DATA_DIR);
//    data_1 += "/geo_db/example6.db";
//
//	tool_class tc;
//
//
//	return 0;
//}