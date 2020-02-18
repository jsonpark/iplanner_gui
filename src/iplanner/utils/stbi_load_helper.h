#ifndef IPLANNER_UTILS_STBI_LOAD_HELPER_H_
#define IPLANNER_UTILS_STBI_LOAD_HELPER_H_

namespace iplanner
{
void stbi_set_flip_vertically_on_load_helper(int flag_true_if_should_flip);
unsigned char* stbi_load_helper(const char* filename, int* x, int* y, int* comp, int req_comp);
void stbi_image_free_helper(void* retval_from_stbi_load);
}

#endif // IPLANNER_UTILS_STBI_LOAD_HELPER_H_
