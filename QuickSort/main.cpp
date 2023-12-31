#include <GL/glut.h>
#include <iostream>
#include <unistd.h>
#define max 100

struct color {
    int r, g, b;
};

color x[] = {
    {0, 0, 1},
    {1, 0, 0},
    {0, 1, 0},
    {0, 0, 1},
    {1, 1, 0},
    {0, 1, 1},
    {1, 0, 1}
};

struct line {
    int len;
    color c;
};

line ln[max], sortln[max], temp[max];

int min = 0, count = 0, finished = 0;
int sorttype = 0, inptype = 0;

void drawlines();
void input(int);

void delay() {
    usleep(10000);
}

void merge(int l, int m, int r) {
    int i = l, j = m + 1, k = l;
    while (i <= m && j <= r) {
        if (sortln[i].len <= sortln[j].len) {
            temp[k].c.r = sortln[i].c.r;
            temp[k].c.g = sortln[i].c.g;
            temp[k].c.b = sortln[i].c.b;
            temp[k].len = sortln[i].len;
            i++;
            k++;
        } else {
            temp[k].c.r = sortln[j].c.r;
            temp[k].c.g = sortln[j].c.g;
            temp[k].c.b = sortln[j].c.b;
            temp[k].len = sortln[j].len;
            j++;
            k++;
        }
    }
    while (i <= m) {
        temp[k].c.r = sortln[i].c.r;
        temp[k].c.g = sortln[i].c.g;
        temp[k].c.b = sortln[i].c.b;
        temp[k].len = sortln[i].len;
        i++;
        k++;
    }
    while (j <= r) {
        temp[k].c.r = sortln[j].c.r;
        temp[k].c.g = sortln[j].c.g;
        temp[k].c.b = sortln[j].c.b;
        temp[k].len = sortln[j].len;
        j++;
        k++;
    }
    for (i = l; i <= r; i++) {
        sortln[i].c.r = temp[i].c.r;
        sortln[i].c.g = temp[i].c.g;
        sortln[i].c.b = temp[i].c.b;
        sortln[i].len = temp[i].len;
        glClear(GL_COLOR_BUFFER_BIT);
        drawlines();
        glutSwapBuffers();
        delay();
    }
}

void mergesort(int l, int r) {
    if (l < r) {
        int m = (l + r) / 2;
        mergesort(l, m);
        mergesort(m + 1, r);
        merge(l, m, r);
    }
}

int partition(int l, int r) {
    line temp;
    int i, j, pivot;
    pivot = sortln[l].len;
    i = l + 1;
    j = r;
    while (i <= j) {
        while (pivot >= sortln[i].len && i <= r)
            i++;
        while (pivot <= sortln[j].len && j > l)
            j--;
        if (i < j) {
            temp = sortln[i];
            sortln[i] = sortln[j];
            sortln[j] = temp;
            glClear(GL_COLOR_BUFFER_BIT);
            drawlines();
            glutSwapBuffers();
            delay();
        }
    }
    temp = sortln[l];
    sortln[l] = sortln[j];
    sortln[j] = temp;
    glClear(GL_COLOR_BUFFER_BIT);
    drawlines();
    glutSwapBuffers();
    delay();
    return j;
}

void quicksort(int l, int r) {
    int pos;
    if (l < r) {
        pos = partition(l, r);
        quicksort(l, pos - 1);
        quicksort(pos + 1, r);
    }
}

void insertion() {
    int i, j;
    line temp;
    for (i = 1; i < max; i++) {
        temp = sortln[i];
        j = i - 1;
        while (j >= 0 && sortln[j].len > temp.len) {
            sortln[j + 1] = sortln[j];
            j = j - 1;
            glClear(GL_COLOR_BUFFER_BIT);
            drawlines();
            glutSwapBuffers();
            delay();
        }
        sortln[j + 1] = temp;
        glClear(GL_COLOR_BUFFER_BIT);
        drawlines();
        glutSwapBuffers();
        delay();
    }
}

void sort() {
    line temp;
    int a = 0;
    switch (sorttype) {
        case 0:
            for (int j = 0; j < max - 1; j++) {
                if (sortln[j].len > sortln[j + 1].len) {
                    temp = sortln[j];
                    sortln[j] = sortln[j + 1];
                    sortln[j + 1] = temp;
                    glutPostRedisplay();
                    delay();
                    break;
                }
            }
            break;
        case 1:
            min = count;
            for (int i = count + 1; i < max; i++) {
                if (sortln[min].len > sortln[i].len) {
                    min = i;
                }
            }
            temp = sortln[min];
            sortln[min] = sortln[count];
            sortln[count] = temp;
            count++;
            glutPostRedisplay();
            delay();
            break;
        case 2:
            mergesort(0, max - 1);
            break;
        case 3:
            quicksort(0, max - 1);
            break;
        case 4:
            insertion();
            break;
    }
}

void drawlines() {
    int start = 50;
    for (int i = 0; i < max; i++) {
        glColor3f(sortln[i].c.r, sortln[i].c.g, sortln[i].c.b);
        glBegin(GL_LINES);
        glVertex2f(start, 10);
        glVertex2f(start, 10 + sortln[i].len);
        glEnd();
        start += 10;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, max * 10 + 100, 0, 550);
    glLineWidth(3);
    drawlines();
    glutSwapBuffers();
    sort();
}

void randomize() {
    int a;
    for (int i = 0; i < max; i++) {
        ln[i].len = (rand() % 500);
        a = rand() % 7;
        ln[i].c = x[a];
    }
}

void choice(int ch) {
    sorttype = ch;
    count = 0;
    min = 0;
    for (int i = 0; i < max; i++)
        sortln[i] = ln[i];
    display();
}

int main(int argc, char** argv) {
    randomize();
    for (int i = 0; i < max; i++)
        sortln[i] = ln[i];
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowPosition(250, 50);
    glutInitWindowSize(900, 600);
    glutCreateWindow("SORTING VISUALIZER");
    glutCreateMenu(choice);
    glutAddMenuEntry("Bubble sort", 0);
    glutAddMenuEntry("Selection sort", 1);
    glutAddMenuEntry("Merge sort", 2);
    glutAddMenuEntry("Quick sort", 3);
    glutAddMenuEntry("Insertion sort", 4);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutDisplayFunc(display);
    glutMainLoop();
}
