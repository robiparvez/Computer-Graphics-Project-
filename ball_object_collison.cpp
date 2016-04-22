#include<GL/glut.h>
#include<stdio.h>
#include<math.h>

void glutInitRendering(){
    glEnable(GL_DEPTH_TEST);
}
void reshaped(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 0, 1, 200);
}




class Object{
    public:
    float x;
    float y;
    float leftBound,rightBound,topBound,bottomBound;
    float motionDirection=0;
    float motionSpeed=.0001;
    void changeX(float change){
        x+=change;
    }
    void changeY(float change){
        y+=change;
    }
    void flipMotionDirection(){
        if(motionDirection==0) motionDirection=1;
        else motionDirection=0;
    }
    void setLocation(float x, float y){
        this->x=x;
        this->y=y;
    }
    void applyMotion(){
        if(motionDirection==0){
            y-=motionSpeed;
        }else{
            y+=motionSpeed;
        }
    }
    void setBounds(float leftBound, float topBound, float rightBound, float bottomBound){
        this->leftBound=leftBound;
        this->rightBound=rightBound;
        this->topBound=topBound;
        this->bottomBound=bottomBound;
    }
    void drawBoundingBox(){
        glColor3f(1,1,0);
        glPushMatrix();
            glTranslatef(x,y,0);
            glLineWidth(1);
            glBegin(GL_LINE_LOOP);
                glVertex2f(leftBound,topBound);
                glVertex2f(rightBound,topBound);
                glVertex2f(rightBound,bottomBound);
                glVertex2f(leftBound,bottomBound);
            glEnd();
        glPopMatrix();
    }
    bool detectCollision(Object object){
        bool collision=false;
        bool collisionX=true;
        bool collisionY=true;

        if(object.leftBound+object.x<this->leftBound+this->x && object.rightBound+object.x<this->leftBound+this->x){
            collisionX=false;
        }else if(object.leftBound+object.x>this->rightBound+this->x && object.rightBound+object.x>this->rightBound+this->x){
            collisionX=false;
        }
        if(object.topBound+object.y>this->topBound+this->y && object.bottomBound+object.y>this->topBound+this->y){
            collisionY=false;
        }else if(object.topBound+object.y<this->bottomBound+this->y && object.bottomBound+object.y<this->bottomBound+this->y){
            collisionY=false;
        }

        if(collisionX && collisionY) collision=true;

        if(!collision) collision=detectScreenCollision();

        return collision;
        //if(collision) this->flipMotionDirection();
    }
    bool detectScreenCollision(){
        bool collision=false;
        if(this->topBound+this->y>1) collision=true;
        if(this->bottomBound+this->y<-1) collision=true;
        if(this->leftBound+this->x<-1) collision=true;
        if(this->rightBound+this->x>1) collision=true;
        return collision;
    }
    void draw(){}
};
class Box: public Object{
    public:
    float r=1;
    void setRadius(float radius){
        r=radius;
        leftBound=-radius;
        topBound=radius;
        rightBound=radius;
        bottomBound=-radius;
    }
    void draw(){
        glColor3f(0,1,0);
        glPushMatrix();
            glTranslatef(x,y,0);
            glBegin(GL_TRIANGLES);
                glVertex2f(-r,r);
                glVertex2f(r,r);
                glVertex2f(r,-r);
            glEnd();
            glBegin(GL_TRIANGLES);
                glVertex2f(-r,r);
                glVertex2f(-r,-r);
                glVertex2f(r,-r);
            glEnd();
        glPopMatrix();
    }
};
class Circle: public Object{
    public:
    float r=1;
    void setRadius(float radius){
        r=radius;
        leftBound=-radius;
        topBound=radius;
        rightBound=radius;
        bottomBound=-radius;
    }
    void draw(){
        float num_segments=100;
        glColor3f(1,0,0);
        glPushMatrix();
            glTranslatef(x,y,0);
            glBegin(GL_TRIANGLE_FAN);
                for(int ii = 0; ii < num_segments; ii++){
                    float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle

                    float x = r * cosf(theta);//calculate the x component
                    float y = r * sinf(theta);//calculate the y component

                    glVertex2f(x, y);//output vertex

                }
            glEnd();
        glPopMatrix();
    }
};
Box box1;
Circle circle1;

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);

    circle1.applyMotion();
    if(circle1.detectCollision(box1)) circle1.flipMotionDirection();
    circle1.draw();
    box1.draw();

    glutSwapBuffers();
}

void mykey(unsigned char key, int x, int y){
    if(key == 'q' || key == 'Q'){
        exit(0);
    }

    if(key == 'w' || key == 'W'){
        float changeValue=.05;
        circle1.changeY(changeValue);
        if(circle1.detectCollision(box1)){
            circle1.changeY(-changeValue);
        }
    }
    if(key == 's' || key == 'S'){
        float changeValue=-.05;
        circle1.changeY(changeValue);
        if(circle1.detectCollision(box1)){
            circle1.changeY(-changeValue);
        }
    }
    if(key == 'a' || key == 'A'){
        float changeValue=-.05;
        circle1.changeX(changeValue);
        if(circle1.detectCollision(box1)){
            circle1.changeX(-changeValue);
        }
    }
    if(key == 'd' || key == 'D'){
        float changeValue=.05;
        circle1.changeX(changeValue);
        if(circle1.detectCollision(box1)){
            circle1.changeX(-changeValue);
        }
    }

    if(key == 'i' || key == 'W'){
        float changeValue=.05;
        box1.changeY(changeValue);
        if(box1.detectCollision(circle1)){
            box1.changeY(-changeValue);
        }
    }
    if(key == 'k' || key == 'S'){
        float changeValue=-.05;
        box1.changeY(changeValue);
        if(box1.detectCollision(circle1)){
            box1.changeY(-changeValue);
        }
    }
    if(key == 'j' || key == 'A'){
        float changeValue=-.05;
        box1.changeX(changeValue);
        if(box1.detectCollision(circle1)){
            box1.changeX(-changeValue);
        }
    }
    if(key == 'l' || key == 'D'){
        float changeValue=.05;
        box1.changeX(changeValue);
        if(box1.detectCollision(circle1)){
            box1.changeX(-changeValue);
        }
    }
}

int main(int argc, char **argv){

    box1.setRadius(.1);
    circle1.setRadius(.2);
    circle1.setLocation(0,.7);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Ball Collision");
    glutInitRendering();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc(mykey);
    glutReshapeFunc(reshaped);
    glutMainLoop();
}
