
#if defined(HAVE_TIFF)

#include "catch.hpp"

#include <mapnik/image_reader.hpp>
#include <mapnik/image_util.hpp>
#include <mapnik/util/file_io.hpp>

#include <mapnik/tiff_io.hpp>
#include "../../src/tiff_reader.cpp"

#define TIFF_ASSERT(filename) \
    mapnik::tiff_reader<boost::iostreams::file_source> tiff_reader(filename); \
    REQUIRE( tiff_reader.width() == 256 ); \
    REQUIRE( tiff_reader.height() == 256 ); \
    REQUIRE( tiff_reader.planar_config() == PLANARCONFIG_CONTIG ); \
    std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(filename,"tiff")); \
    REQUIRE( reader->width() == 256 ); \
    REQUIRE( reader->height() == 256 ); \
    mapnik::util::file file(filename); \
    mapnik::tiff_reader<boost::iostreams::array_source> tiff_reader2(file.data().get(),file.size()); \
    REQUIRE( tiff_reader2.width() == 256 ); \
    REQUIRE( tiff_reader2.height() == 256 ); \
    std::unique_ptr<mapnik::image_reader> reader2(mapnik::get_image_reader(file.data().get(),file.size())); \
    REQUIRE( reader2->width() == 256 ); \
    REQUIRE( reader2->height() == 256 ); \

#define TIFF_ASSERT_ALPHA \
    REQUIRE( tiff_reader.has_alpha() == true ); \
    REQUIRE( tiff_reader.premultiplied_alpha() == false ); \
    REQUIRE( reader->has_alpha() == true ); \
    REQUIRE( reader->premultiplied_alpha() == false ); \
    REQUIRE( tiff_reader2.has_alpha() == true ); \
    REQUIRE( tiff_reader2.premultiplied_alpha() == false ); \
    REQUIRE( reader2->has_alpha() == true ); \
    REQUIRE( reader2->premultiplied_alpha() == false ); \

#define TIFF_ASSERT_NO_ALPHA \
    REQUIRE( tiff_reader.has_alpha() == false ); \
    REQUIRE( tiff_reader.premultiplied_alpha() == false ); \
    REQUIRE( reader->has_alpha() == false ); \
    REQUIRE( reader->premultiplied_alpha() == false ); \
    REQUIRE( tiff_reader2.has_alpha() == false ); \
    REQUIRE( tiff_reader2.premultiplied_alpha() == false ); \
    REQUIRE( reader2->has_alpha() == false ); \
    REQUIRE( reader2->premultiplied_alpha() == false ); \

#define TIFF_ASSERT_SIZE( data,reader ) \
    REQUIRE( data.width() == reader->width() ); \
    REQUIRE( data.height() == reader->height() ); \

#define TIFF_READ_ONE_PIXEL \
    mapnik::image_data_any subimage = reader->read(1, 1, 1, 1); \
    REQUIRE( subimage.width() == 1 ); \
    REQUIRE( subimage.height() == 1 ); \

TEST_CASE("tiff io") {

SECTION("scan rgb8 striped") {
    std::string filename("./tests/data/tiff/scan_512x512_rgb8_striped.tif");
    mapnik::tiff_reader<boost::iostreams::file_source> tiff_reader(filename);
    REQUIRE( tiff_reader.width() == 512 );
    REQUIRE( tiff_reader.height() == 512 );
    REQUIRE( tiff_reader.planar_config() == PLANARCONFIG_CONTIG );
    REQUIRE( tiff_reader.rows_per_strip() == 16 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == false );
    REQUIRE( tiff_reader.tile_width() == 0 );
    REQUIRE( tiff_reader.tile_height() == 0 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_PALETTE );
    REQUIRE( tiff_reader.compression() == COMPRESSION_NONE );
    std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(filename,"tiff"));
    REQUIRE( reader->width() == 512 );
    REQUIRE( reader->height() == 512 );
    mapnik::util::file file(filename);
    mapnik::tiff_reader<boost::iostreams::array_source> tiff_reader2(file.data().get(),file.size());
    REQUIRE( tiff_reader2.width() == 512 );
    REQUIRE( tiff_reader2.height() == 512 );
    std::unique_ptr<mapnik::image_reader> reader2(mapnik::get_image_reader(file.data().get(),file.size()));
    REQUIRE( reader2->width() == 512 );
    REQUIRE( reader2->height() == 512 );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_rgba8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("scan rgb8 tiled") {
    std::string filename("./tests/data/tiff/scan_512x512_rgb8_tiled.tif");
    mapnik::tiff_reader<boost::iostreams::file_source> tiff_reader(filename);
    REQUIRE( tiff_reader.width() == 512 );
    REQUIRE( tiff_reader.height() == 512 );
    REQUIRE( tiff_reader.planar_config() == PLANARCONFIG_CONTIG );
    REQUIRE( tiff_reader.rows_per_strip() == 0 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == true );
    REQUIRE( tiff_reader.tile_width() == 256 );
    REQUIRE( tiff_reader.tile_height() == 256 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_PALETTE );
    REQUIRE( tiff_reader.compression() == COMPRESSION_LZW );
    std::unique_ptr<mapnik::image_reader> reader(mapnik::get_image_reader(filename,"tiff"));
    REQUIRE( reader->width() == 512 );
    REQUIRE( reader->height() == 512 );
    mapnik::util::file file(filename);
    mapnik::tiff_reader<boost::iostreams::array_source> tiff_reader2(file.data().get(),file.size());
    REQUIRE( tiff_reader2.width() == 512 );
    REQUIRE( tiff_reader2.height() == 512 );
    std::unique_ptr<mapnik::image_reader> reader2(mapnik::get_image_reader(file.data().get(),file.size()));
    REQUIRE( reader2->width() == 512 );
    REQUIRE( reader2->height() == 512 );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_rgba8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("rgba8 striped") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_rgba8_striped.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 1 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == false );
    REQUIRE( tiff_reader.tile_width() == 0 );
    REQUIRE( tiff_reader.tile_height() == 0 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_RGB );
    REQUIRE( tiff_reader.compression() == COMPRESSION_ADOBE_DEFLATE );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_rgba8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("rgba8 tiled") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_rgba8_tiled.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 0 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == true );
    REQUIRE( tiff_reader.tile_width() == 256 );
    REQUIRE( tiff_reader.tile_height() == 256 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_RGB );
    REQUIRE( tiff_reader.compression() == COMPRESSION_LZW );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_rgba8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("rgb8 striped") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_rgb8_striped.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 10 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == false );
    REQUIRE( tiff_reader.tile_width() == 0 );
    REQUIRE( tiff_reader.tile_height() == 0 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_RGB );
    REQUIRE( tiff_reader.compression() == COMPRESSION_NONE );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_rgba8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("rgb8 tiled") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_rgb8_tiled.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 0 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == true );
    REQUIRE( tiff_reader.tile_width() == 256 );
    REQUIRE( tiff_reader.tile_height() == 256 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_RGB );
    REQUIRE( tiff_reader.compression() == COMPRESSION_LZW );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_rgba8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("gray8 striped") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_gray8_striped.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 32 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == false );
    REQUIRE( tiff_reader.tile_width() == 0 );
    REQUIRE( tiff_reader.tile_height() == 0 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_MINISBLACK );
    REQUIRE( tiff_reader.compression() == COMPRESSION_NONE );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_gray8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("gray8 tiled") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_gray8_tiled.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 0 );
    REQUIRE( tiff_reader.bits_per_sample() == 8 );
    REQUIRE( tiff_reader.is_tiled() == true );
    REQUIRE( tiff_reader.tile_width() == 256 );
    REQUIRE( tiff_reader.tile_height() == 256 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_MINISBLACK );
    REQUIRE( tiff_reader.compression() == COMPRESSION_LZW );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_gray8>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("gray16 striped") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_gray16_striped.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 16 );
    REQUIRE( tiff_reader.bits_per_sample() == 16 );
    REQUIRE( tiff_reader.is_tiled() == false );
    REQUIRE( tiff_reader.tile_width() == 0 );
    REQUIRE( tiff_reader.tile_height() == 0 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_MINISBLACK );
    REQUIRE( tiff_reader.compression() == COMPRESSION_NONE );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_gray16>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("gray16 tiled") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_gray16_tiled.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 0 );
    REQUIRE( tiff_reader.bits_per_sample() == 16 );
    REQUIRE( tiff_reader.is_tiled() == true );
    REQUIRE( tiff_reader.tile_width() == 256 );
    REQUIRE( tiff_reader.tile_height() == 256 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_MINISBLACK );
    REQUIRE( tiff_reader.compression() == COMPRESSION_LZW );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_gray16>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("gray32f striped") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_gray32f_striped.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 8 );
    REQUIRE( tiff_reader.bits_per_sample() == 32 );
    REQUIRE( tiff_reader.is_tiled() == false );
    REQUIRE( tiff_reader.tile_width() == 0 );
    REQUIRE( tiff_reader.tile_height() == 0 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_MINISBLACK );
    REQUIRE( tiff_reader.compression() == COMPRESSION_NONE );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_gray32f>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

SECTION("gray32f tiled") {
    TIFF_ASSERT("./tests/data/tiff/ndvi_256x256_gray32f_tiled.tif")
    REQUIRE( tiff_reader.rows_per_strip() == 0 );
    REQUIRE( tiff_reader.bits_per_sample() == 32 );
    REQUIRE( tiff_reader.is_tiled() == true );
    REQUIRE( tiff_reader.tile_width() == 256 );
    REQUIRE( tiff_reader.tile_height() == 256 );
    REQUIRE( tiff_reader.photometric() == PHOTOMETRIC_MINISBLACK );
    REQUIRE( tiff_reader.compression() == COMPRESSION_LZW );
    mapnik::image_data_any data = reader->read(0, 0, reader->width(), reader->height());
    REQUIRE( data.is<mapnik::image_data_gray32f>() == true );
    TIFF_ASSERT_SIZE( data,reader );
    TIFF_ASSERT_NO_ALPHA
    TIFF_READ_ONE_PIXEL
}

}

#endif
