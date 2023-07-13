#include <cyng.h>

#include <reader.h>

#include <cyng/io/ostream.h>
#include <cyng/log/log.h>
#include <cyng/log/logger.h>
#include <cyng/log/record.h>
#include <cyng/task/controller.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdlib.h> //  EXIT_SUCCESS

#include <boost/program_options.hpp>

int main(int argc, char *argv[]) {

    std::cout << argv[0] << std::endl;

    //
    // get the default configuration file name
    //
    std::string config_file = std::string(cyng::CYNG_PROJECT_NAME) + ".cfg";
    auto const here = std::filesystem::current_path();
    std::string inp_file = "main.asm";
    std::string out_file = (here / "out.cyng").string();
#ifdef _DEBUG
    auto log_level_str = cyng::to_string(cyng::severity::LEVEL_TRACE);
#else
    auto log_level_str = cyng::to_string(cyng::severity::LEVEL_INFO);
#endif
    std::size_t pool_size = std::min<std::size_t>(std::thread::hardware_concurrency(), 4) * 2ul;

    //
    //	generic options
    //
    boost::program_options::options_description generic("Generic options");
    generic.add_options()

        ("help,h", "print usage message")("version,v", "print version string")                                                   //
        ("config,C", boost::program_options::value<std::string>(&config_file)->default_value(config_file), "configuration file") //
        ("log-level,L",
         boost::program_options::value<std::string>(&log_level_str)->default_value(log_level_str),
         "log levels are T[RACE], D[EBUG], I[NFO], W[ARNING], E[RROR] and F[ATAL]") //  specify logging level
        ;

    //
    //	all compiler options
    //
    boost::program_options::options_description assembler("assembler");
    assembler.add_options()

        //	verbose level
        ("verbose,V", boost::program_options::value<int>()->default_value(0)->implicit_value(1), "verbose level") //
        ("include-path,I",
         boost::program_options::value<std::vector<std::string>>()->default_value(
             std::vector<std::string>(1, here.string()), here.string()),
         "include paths") // --include-path
        ;

    //
    //	all you can grab from the command line
    //
    boost::program_options::options_description cmdline_options;
    cmdline_options.add(generic).add(assembler);

    //
    //	positional arguments
    //
    boost::program_options::positional_options_description p;
    p.add("source", -1);

    boost::program_options::variables_map vm;
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv).options(cmdline_options).positional(p).run(), vm);
    boost::program_options::notify(vm);

    if (vm.count("help")) {
        std::cout << cmdline_options << std::endl;
        return EXIT_SUCCESS;
    }

    if (vm.count("version")) {
        std::cout << "cyng assembler " << cyng::CYNG_VERSION_NAME << std::endl;
        return EXIT_SUCCESS;
    }

    std::ifstream ifs(config_file);
    if (!ifs) {
        std::cout << "***warn : config file " << config_file << " not found" << std::endl;
    } else {
        //
        //	options available from config file
        //
        boost::program_options::options_description file_options;
        file_options.add_options()(
            "pool-size,P", boost::program_options::value(&pool_size)->default_value(pool_size), "Thread pool size");
        file_options.add(assembler);

        boost::program_options::store(boost::program_options::parse_config_file(ifs, file_options), vm);
        boost::program_options::notify(vm);
    }

    //
    // create a logger facility
    //
    cyng::controller ctl(pool_size);
    cyng::logger _(ctl.create_channel<cyng::log>().first);
    _.start_console_logger();

    //
    //  verbose level
    //
    auto const verbose = vm["verbose"].as<int>();

    //
    //  additional info
    //
    if (verbose > 1) {
        CYNG_LOG_TRACE(_, "verbose level: " << verbose);
    }

    //
    //  Derive the name of the output file from the input file if not otherwise specified
    //
    auto const output_path =
        out_file.empty() ? std::filesystem::path(inp_file).replace_extension("asm") : std::filesystem::path(out_file);

    //
    //  start assembler
    //
    cyng::assembler::reader r(_);
    r.start(inp_file);

    ctl.stop(); //  remove work item
    _.stop();
    return EXIT_SUCCESS;
}
