/*
  ImageConvert.h
  Copyright (c) 2020 Dale Giancono. All rights reserved..

  This class implements a way to convert convert raw images

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/*****************************************************************************/
/*INLCUDE GUARD                                                              */
/*****************************************************************************/
#ifndef IMAGECONVERT_H_
#define IMAGECONVERT_H_

/*****************************************************************************/
/*INLCUDES                                                                   */
/*****************************************************************************/
#include "Image.h"
#include <VmbCPP/VmbCPP.h>
#include <VmbImageTransform/VmbTransform.h>

/*****************************************************************************/
/*MACROS                                                                     */
/*****************************************************************************/

/*****************************************************************************/
/*GLOBAL Data                                                                */
/*****************************************************************************/

/*****************************************************************************/
/*CLASS DECLARATION                                                          */
/*****************************************************************************/
class ImageConvert
{
    public:
      static void convert(
        Image& image, 
        uint32_t width, 
        uint32_t height,
        uint8_t bitDepth);

    private:
};

void ImageConvert::convert(
  Image& image, 
  uint32_t width, 
  uint32_t height,
  uint8_t bitDepth) 
{
  VmbError_t error;
  VmbPixelFormat_t sourceFormat;
  VmbImage sourceImage;
  Image newImage;
  VmbImage destinationImage;
  /* Packed formats not yet supported */

  if(8 == image.getBitDepth())
  {
      // std::cout << "Source Format: VmbPixelFormatMono8" << std::endl;
      sourceFormat = VmbPixelFormatMono8;
  }
  else if(10 == image.getBitDepth())
  {
      if(true == image.isPacked())
      {
        // std::cout << "Source Format: VmbPixelFormatMono10p" << std::endl;
        sourceFormat = VmbPixelFormatMono10p;
      }
      else
      {
        // std::cout << "Source Format: VmbPixelFormatMono10" << std::endl;
        sourceFormat = VmbPixelFormatMono10;
      }
  }  
  else if(12 == image.getBitDepth())
  {
      if(true == image.isPacked())
      {
        // std::cout << "Source Format: VmbPixelFormatMono12p" << std::endl;
        sourceFormat = VmbPixelFormatMono12p;
      }
      else
      {
        // std::cout << "Source Format: VmbPixelFormatMono12" << std::endl;
        sourceFormat = VmbPixelFormatMono12;
      }         
  }
  else 
  {
    // printf("image.bitDepth(): %d\n", image.getBitDepth());
    assert(false && "Bit depth not currently supported");
  }

  sourceImage.Size = sizeof(sourceImage);
  sourceImage.Data = image.getBuffer();

  error = VmbSetImageInfoFromPixelFormat(
    sourceFormat,
    image.getWidth(),
    image.getHeight(),
    &sourceImage);
  assert(error == VmbErrorSuccess);

  // std::cout << "Set image info from pixel format" << std::endl;

  destinationImage.Size = sizeof(destinationImage);
  if(bitDepth <= 8)
  {
    destinationImage.Data = malloc(width*height);
  }
  else
  {
    destinationImage.Data = malloc(width*height*2);
  }

  assert(NULL != destinationImage.Data);

  error = VmbSetImageInfoFromInputParameters(
    sourceFormat,
    width,
    height,
    VmbPixelLayoutMono,
    bitDepth,
    &destinationImage);
  if(error != VmbErrorSuccess)
  {
    std::cerr << "error: " << error <<  std::endl;
  }
  // std::cout << "Set image info from input parameters" << std::endl;

  assert(error == VmbErrorSuccess);

  error = VmbImageTransform(
    &sourceImage, 
    &destinationImage, 
    NULL, 
    0);
  if(error != VmbErrorSuccess)
  {
    std::cerr << "error: " << error <<  std::endl;
  }
  assert(error == VmbErrorSuccess);
  // std::cout << "Did image transform" << std::endl;

  if(bitDepth <= 8)
  {
    // std::cout << "Set the buffer for 8 bit data" << std::endl;

    image.setBuffer(
      destinationImage.Data, 
      width*height);
  }
  else
  {
    // std::cout << "Set the buffer for 16 bit data" << std::endl;
    image.setBuffer(
      destinationImage.Data, 
      width*height*2);
  }



  image.setBitDepth(bitDepth);
  image.setHeight(height);
  image.setWidth(width);
  image.setPackedStatus(false);
  // std:: cout << "destinationImage.ImageInfo.PixelInfo.BitsPerPixel: " << destinationImage.ImageInfo.PixelInfo.BitsPerPixel << std::endl;

  // std::cout << "Set other things" << std::endl;
  free(destinationImage.Data);
  // std::cout << "Freed the data" << std::endl;
}

#endif /* IMAGECONVERT_H_ */