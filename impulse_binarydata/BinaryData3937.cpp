/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#include <cstring>

namespace BinaryData
{

//================== test.ipynb ==================
static const unsigned char temp_binary_data_3936[] =
"{\n"
"  \"cells\": [\n"
"    {\n"
"      \"cell_type\": \"code\",\n"
"      \"execution_count\": 6,\n"
"      \"metadata\": {\n"
"        \"id\": \"YHUH5YOO9dSy\"\n"
"      },\n"
"      \"outputs\": [],\n"
"      \"source\": [\n"
"        \"import os\\n\",\n"
"        \"FS = '96000'\\n\",\n"
"        \"\\n\",\n"
"        \"def rename(src, s_dir):\\n\",\n"
"        \"\\n\",\n"
"        \"\\n\",\n"
"        \"    name = src.replace('_4x12', '')\\n\",\n"
"        \"\\n\",\n"
"        \"\\n\",\n"
"        \"    # print(s_dir +'/'+ src)\\n\",\n"
"        \"    # print(s_dir +'/'+ name)\\n\",\n"
"        \"\\n\",\n"
"        \"    os.rename(s_dir +'/'+ src, s_dir +'/'+ name)\\n\",\n"
"        \"\\n\",\n"
"        \"    # if src.endswith(name):\\n\",\n"
"        \"    #     fx = str(idx) + \\\"_\\\" + item + src[-9:-4] + \\\".csv\\\"\\n\",\n"
"        \"    #     print(src, fx)\\n\",\n"
"        \"    #     os.rename(DIR +'/'+ src, DIR +'/'+ fx)\\n\",\n"
"        \"    # name = name[:-9] + \\\"_train.csv\\\"\\n\",\n"
"        \"\\n\",\n"
"        \"    # if src.endswith(name):\\n\",\n"
"        \"    #     fx = str(idx) + \\\"_\\\" + item + src[-10:-4] + \\\".csv\\\"\\n\",\n"
"        \"    #     print(src, fx)\\n\",\n"
"        \"    #     os.rename(DIR +'/'+ src, DIR +'/'+ fx)\\n\",\n"
"        \"\\n\",\n"
"        \"for target in os.listdir(FS + '/Excel 4x12'):\\n\",\n"
"        \"        rename(target, FS + '/Excel 4x12')\\n\"\n"
"      ]\n"
"    }\n"
"  ],\n"
"  \"metadata\": {\n"
"    \"colab\": {\n"
"      \"name\": \"torchaudio-tutorial.ipynb\",\n"
"      \"provenance\": []\n"
"    },\n"
"    \"kernelspec\": {\n"
"      \"display_name\": \"Python 3\",\n"
"      \"name\": \"python3\"\n"
"    },\n"
"    \"language_info\": {\n"
"      \"codemirror_mode\": {\n"
"        \"name\": \"ipython\",\n"
"        \"version\": 3\n"
"      },\n"
"      \"file_extension\": \".py\",\n"
"      \"mimetype\": \"text/x-python\",\n"
"      \"name\": \"python\",\n"
"      \"nbconvert_exporter\": \"python\",\n"
"      \"pygments_lexer\": \"ipython3\",\n"
"      \"version\": \"3.10.10\"\n"
"    }\n"
"  },\n"
"  \"nbformat\": 4,\n"
"  \"nbformat_minor\": 0\n"
"}\n";

const char* test_ipynb = (const char*) temp_binary_data_3936;
}
