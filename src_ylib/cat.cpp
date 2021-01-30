#include<iostream>
#include<fstream>
#include<filesystem>
#include<string>
#include<vector>
#include<unordered_set>

void test_cat_files(const std::string& cat_file, const std::string& tmp_file)
{
    // *** Why absolute path does not work? Due to WSL? *** //
    std::filesystem::path input(cat_file);
    std::filesystem::path output(tmp_file);
    

    std::string tag("// ***** TAG_FOR_FILE_CAT ***** //");
    std::vector<std::string> undesired_folders;
    undesired_folders.push_back("build/debug");
    undesired_folders.push_back("build/release");

    std::unordered_set<std::string> extensions;
    extensions.insert(".cpp");
    extensions.insert(".h");
    extensions.insert(".sh");
    extensions.insert(".md");  // such as README.md
    extensions.insert(".txt"); // such as CMakeLists.txt


    if (!std::filesystem::exists(input))        { std::cout << "\nFilepath not exists";       return; }
    if (!std::filesystem::is_directory(input))  { std::cout << "\nFilepath not a directory";  return; }
    std::ofstream ofs(output);

    std::uint32_t num_file = 0;
    std::uint32_t num_ctrlM = 0;
    for(const auto& x : std::filesystem::recursive_directory_iterator(input))
    {
        // We can ostream std::filesystem::path directly, but it is double quoted.
        std::filesystem::path path = x.path();                 
        std::filesystem::path dir  = path.parent_path();           // parent directory, like linux command "dirname" 

        // Get std::string from std::fs::path 
        std::string str_fp = path.string();                        // whole filepath
        std::string str_fn = path.filename();                      // only filename, like linux command "basename"

        if      (std::filesystem::is_directory(x.status()))        std::cout << "\nfolder  " << str_fp; // str_fn;
        else if (std::filesystem::is_regular_file(x.status()))     std::cout << "\nfile    " << str_fp; // str_fn;
        else                                                       std::cout << "\nunknown " << str_fp; // str_fn;

        if (std::filesystem::is_regular_file(x.status()))
        {           
            // ************************************** //
            // *** Checking 1 : undesired folders *** //
            // ************************************** //
            bool inside_undesired_folders = false;
            for(const auto& x:undesired_folders)
            {
                if (path.string().find(x) != std::string::npos)
                {
                    inside_undesired_folders = true;
                    break;
                }
            }
            if (inside_undesired_folders) 
            {
                continue; // skip this node
            }

            // ******************************************* //
            // *** Checking 2 : desired file extension *** //
            // ******************************************* //
            if (extensions.find(path.extension()) == extensions.end()) 
            {
                continue; // skip this node
            }

            // ******************************************* //
            // *** Trivial checking : extract filename *** //
            // ******************************************* //
            if (path.string().find(input.string()) != 0)
            {
                std::cout << " ...... BUG";
                continue; // skip this node
            } 
            std::string relative_path = path.string().substr(input.string().size());


            // ********************************** //
            // *** Confirmed to add this node *** //
            // ********************************** //
            std::cout << " ...... this node is added";
            ++num_file;

            std::ifstream ifs(path);
            ofs << "\n" << tag;
            ofs << "\n// " << relative_path; 

        
            // ***************************************** //
            // *** Approach 1 : Trimming is possible *** //
            // ***************************************** //
            while(ifs.good())
            {
                std::string str;
                std::getline(ifs, str);

                // Erase ctrl-M character for windows-linux-subsystem
                auto pos = str.find('\r'); 
                if (pos != std::string::npos)
                {
                    ++num_ctrlM;
                    str.erase(pos,1); 
                }
                ofs << "\n" << str;
            }

            // *********************************************** //
            // *** Approach 2 : No trimming, but one-liner *** //
            // *********************************************** //
        //  ofs << ifs.rdbuf();
        }
    }

    std::cout << "\n*** Completed *** ";
    std::cout << "\nfile  count = " << num_file;
    std::cout << "\nctrlM count = " << num_ctrlM;
    std::cout << "\n\n";
}


// ************************************************************************** // 
// Two create directory functions :
//     std::filesystem::create_directory  (const std::filesystem::path&);  
//     std::filesystem::create_directories(const std::filesystem::path&);
//
// Both functions work even if the directory already exist.
// The former does not support recursive creation, the latter does.
// ************************************************************************** // 
// Two getline functions :
//     std::getline()
//     std::istream::getline()
//
// The former supports std::string, the latter does not, need char* & length.
// ************************************************************************** // 
void test_cut_files(const std::string& tmp_file, const std::string& cut_file)
{
    std::filesystem::path input(tmp_file);
    std::filesystem::path output(cut_file);
    std::filesystem::remove_all(output);


    std::string tag("// ***** TAG_FOR_FILE_CAT ***** //");
    std::ifstream ifs(input);
    std::ofstream ofs;

    std::uint32_t num_file = 0;
    while(ifs.good())
    {
        std::string str;
        std::getline(ifs, str); 

        // *********** //
        // *** Tag *** //
        // *********** //
        if (str.find(tag)!=std::string::npos)
        {
            if (ofs.is_open())
            {
                ofs.close();
            }

            // *** Load filename *** //
            std::getline(ifs, str); 
            if (str.size() <= 3)   throw std::runtime_error("incorrect file format 0");
            if (str[0] != '/')     throw std::runtime_error("incorrect file format 1");
            if (str[1] != '/')     throw std::runtime_error("incorrect file format 2");
            if (str[2] != ' ')     throw std::runtime_error("incorrect file format 3");
            
            // *** Create folder recursively *** //
            std::filesystem::path complete_path(output.string() + str.substr(3));
            std::filesystem::create_directories(complete_path.parent_path());

            // *** Write to file *** //
            ofs.open(complete_path);
            ++num_file;
        }
        // *************** //
        // *** Non tag *** //
        // *************** //
        else
        {
            if (ofs.is_open())
            {
                ofs << "\n" << str;
            }
        }
    }    

    std::cout << "\n*** Completed *** ";
    std::cout << "\nfile  count = " << num_file;
}

void test_cat_cut()
{
    // Does not support absolute path, it throws runtime_error "file not found"
//  test_cat_files("../../../YExperimental_20201030", "../../../cat_and_cut/tmp");
//  test_cut_files("../../../cat_and_cut/tmp",        "../../../cat_and_cut/extract");

    // For extraction of YLib files 
    test_cut_files("../../../cat_and_cut/tmp0",       "../../../cat_and_cut/extract0");
    test_cut_files("../../../cat_and_cut/tmp1",       "../../../cat_and_cut/extract1");
}




