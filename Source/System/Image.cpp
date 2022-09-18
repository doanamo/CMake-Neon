#include "Precompiled.hpp"
#include "Image.hpp"

namespace System
{
    Image::~Image() noexcept
    {
        if(!m_data.empty())
        {
            LOG_TRACE("Destroyed image ({} bytes)", m_data.size());
        }
    }

    bool Image::Setup(const SetupFromFile& params)
    {
        ASSERT(m_data.empty());

        if(params.path.extension() != ".png")
        {
            LOG_ERROR("Unsupported image extension in \"{}\" path", params.path);
            return false;
        }

        std::ifstream file(params.path, std::ios::binary);
        if(!file.is_open())
        {
            LOG_ERROR("Failed to open \"{}\" file", params.path);
            return false;
        }

        const png_size_t png_sig_size = 8;
        png_byte png_sig[png_sig_size];

        if(!file.read((char*)png_sig, png_sig_size))
        {
            LOG_ERROR("Failed to read from \"{}\" file", params.path);
            return false;
        }

        if(png_sig_cmp(png_sig, 0, png_sig_size) != 0)
        {
            LOG_ERROR("Invalid PNG signature in \"{}\" file", params.path);
            return false;
        }

        auto png_error_fn = [](png_structp png_ptr, png_const_charp error_msg)
        {
            LOG_ERROR("PNG: {}", error_msg);
        };

        auto png_warn_fn = [](png_structp png_ptr, png_const_charp warn_msg)
        {
            LOG_WARNING("PNG: {}", warn_msg);
        };

        png_structp png_read_ptr = png_create_read_struct(
            PNG_LIBPNG_VER_STRING, nullptr, png_error_fn, png_warn_fn);
        ASSERT(png_read_ptr != nullptr);

        SCOPE_GUARD([&png_read_ptr]
        {
            png_destroy_read_struct(&png_read_ptr, nullptr, nullptr);
        });

        png_infop png_info_ptr = png_create_info_struct(png_read_ptr);
        ASSERT(png_info_ptr != nullptr);

        SCOPE_GUARD([&png_read_ptr, &png_info_ptr]
        {
            png_destroy_info_struct(png_read_ptr, &png_info_ptr);
        });

        auto png_read_fn = [](png_structp png_ptr, png_bytep data, png_size_t length) -> void
        {
            auto* file = (std::ifstream*)png_get_io_ptr(png_ptr);
            if(!file->read((char*)data, length))
            {
                png_error(png_ptr, "Failed to read from file");
            }
        };

        png_bytep* png_row_ptrs = nullptr;

        SCOPE_GUARD([&png_row_ptrs]
        {
            delete[] png_row_ptrs;
        });

        // Apparently a standard way of handling errors with libpng.
        // Library jumps here if one of its functions encounters an error!
        // This is the reason why scope guards and other objects are declared
        // before this call. Be aware of how dangerous it is to do in C++.
        // For example, objects created past these lines will not have their
        // destructor called if the library jumps back here on an error.
        if(setjmp(png_jmpbuf(png_read_ptr)))
        {
            LOG_ERROR("Error occurred while reading from \"{}\" file", params.path);
            return false;
        }

        png_set_read_fn(png_read_ptr, (png_voidp)&file, png_read_fn);
        png_set_sig_bytes(png_read_ptr, png_sig_size);
        png_read_info(png_read_ptr, png_info_ptr);

        png_uint_32 width = png_get_image_width(png_read_ptr, png_info_ptr);
        png_uint_32 height = png_get_image_height(png_read_ptr, png_info_ptr);
        png_uint_32 depth = png_get_bit_depth(png_read_ptr, png_info_ptr);
        png_uint_32 channels = png_get_channels(png_read_ptr, png_info_ptr);
        png_uint_32 format = png_get_color_type(png_read_ptr, png_info_ptr);

        switch(format)
        {
        case PNG_COLOR_TYPE_GRAY:
        case PNG_COLOR_TYPE_GRAY_ALPHA:
            if(depth < 8)
            {
                png_set_expand_gray_1_2_4_to_8(png_read_ptr);
                depth = png_get_bit_depth(png_read_ptr, png_info_ptr);
            }
            break;

        case PNG_COLOR_TYPE_PALETTE:
        {
            png_set_palette_to_rgb(png_read_ptr);
            channels = 3;

            if(png_get_valid(png_read_ptr, png_info_ptr, PNG_INFO_tRNS))
            {
                png_set_tRNS_to_alpha(png_read_ptr);
                channels += 1;
            }

            ASSERT(png_get_channels(png_read_ptr, png_info_ptr) == channels);
        }
        break;

        case PNG_COLOR_TYPE_RGB:
        case PNG_COLOR_TYPE_RGBA:
            break;

        default:
            LOG_ERROR("Unsupported PNG image format");
            return false;
        }

        if(depth == 16)
        {
            png_set_strip_16(png_read_ptr);
            depth = png_get_bit_depth(png_read_ptr, png_info_ptr);
        }

        if(depth != 8)
        {
            LOG_ERROR("Unsupported PNG image depth size");
            return false;
        }

        m_data.resize(width * height * channels);

        png_row_ptrs = new png_bytep[height];
        png_uint_32 png_stride = width * channels;
        for(png_uint_32 i = 0; i < height; ++i)
        {
            // Reverse order of rows to flip image because
            // OpenGL texture coordinates are also flipped.
            png_uint_32 png_row_index = height - i - 1;
            png_uint_32 png_offset = i * png_stride;
            png_row_ptrs[png_row_index] = m_data.data() + png_offset;
        }

        png_read_image(png_read_ptr, png_row_ptrs);

        if(m_data.empty())
        {
            LOG_ERROR("Failed to read any image data from \"{}\" file", params.path);
            return false;
        }

        m_width = width;
        m_height = height;
        m_channels = channels;
 
        LOG_TRACE("Created image ({} bytes)", m_data.size());
        return true;
    }
} // namespace System
