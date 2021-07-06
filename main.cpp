#include <iostream>

#include "tvgLoader.h"
#include "tvgJpgLoader.h"
#include "tvgJpgLoaderTurbo.h"

#define REPLIES 100

void loadRichgel(const string& path) {
    Loader* loader = new JpgLoader();
    loader->open(path);
    loader->read();
    loader->close();
    loader->pixels();
}

void loadTurbo(const string& path) {
    Loader* loader = new JpgLoaderTurbo();
    loader->open(path);
    loader->read();
    loader->close();
    loader->pixels();
}

int main(int argc, const char * argv[]) {
    const string path = "test-image.jpg";

    clock_t begin = clock();
    for (int i = 0; i < REPLIES; i++) {
        loadRichgel(path);
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("loadRichgel(%d) time %0.3fs\n", REPLIES, time_spent);

    begin = clock();
    for (int i = 0; i < REPLIES; i++) {
        loadTurbo(path);
    }
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("loadTurbo(%d) time %0.3fs\n", REPLIES, time_spent);

    return 0;

}
