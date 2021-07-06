/*
 * Copyright (c) 2021 Samsung Electronics Co., Ltd. All rights reserved.

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <memory.h>
#include "tvgJpgLoaderTurbo.h"

/************************************************************************/
/* Internal Class Implementation                                        */
/************************************************************************/

void JpgLoaderTurbo::clear()
{
    tjFree(data);
    data = NULL;
    pointer = NULL;
    size = 0;
}

/************************************************************************/
/* External Class Implementation                                        */
/************************************************************************/

JpgLoaderTurbo::JpgLoaderTurbo()
{
    jpegDecompressor = tjInitDecompress();
}

JpgLoaderTurbo::~JpgLoaderTurbo()
{
    tjDestroy(jpegDecompressor);
    tjFree(image);
    image = NULL;
}

bool JpgLoaderTurbo::open(const string& path)
{
    clear();

    bool success = false;
    FILE *jpegFile = NULL;
    if ((jpegFile = fopen(path.c_str(), "rb")) == NULL) return false;

    //determine size
    if (fseek(jpegFile, 0, SEEK_END) < 0) goto finalize;
    if (((size = ftell(jpegFile)) < 1)) goto finalize;
    if (fseek(jpegFile, 0, SEEK_SET)) goto finalize;

    data = (unsigned char *) tjAlloc(size);
    if (!data) goto finalize;

    if (fread(data, size, 1, jpegFile) < 1) goto failure;

    pointer = data;
    success = true;
    goto finalize;

failure:
    clear();

finalize:
    fclose(jpegFile);
    return success;
}

bool JpgLoaderTurbo::open(const char* data, uint32_t size, bool copy)
{
    clear();

    if (copy) {
        this->data = (unsigned char *) malloc(size);
        if (!this->data) return false;
        memcpy((unsigned char *)this->data, data, size);
        this->pointer = this->data;
    } else {
        this->pointer = (unsigned char *)data;
    }

    this->size = size;
    return true;
}

bool JpgLoaderTurbo::read()
{
    int width, height;

    //decompress header
    int inSubsamp, inColorspace;
    if (tjDecompressHeader3(jpegDecompressor, data, size, &width, &height, &inSubsamp, &inColorspace) < 0) return false;

    //alloc image buffer
    if (image) tjFree(image);
    image = (unsigned char *)tjAlloc(width * height * tjPixelSize[TJPF_BGRX]);
    if (!image) return false;

    //decompress jpg image
    if (tjDecompress2(jpegDecompressor, data, size, image, width, 0, height, TJPF_BGRX, 0) < 0) {
        tjFree(image);
        image = NULL;
        return false;
    }

    vw = w = width;
    vh = h = height;
    return true;
}

bool JpgLoaderTurbo::close()
{
    clear();
    return true;
}

const uint32_t* JpgLoaderTurbo::pixels()
{
    return (const uint32_t*)image;
}
