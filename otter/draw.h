#ifndef draw
#define draw

typedef struct point point;
struct point {
  int x;
  int y;
};

void drawImage(unsigned char *image, int dim);

void drawDot(unsigned int col, unsigned int row, unsigned char RGB);

void drawCoords(point *coordinates, unsigned int numPts, unsigned char RGB);

void splashScreen(int shiftX, int shiftY);

#endif
