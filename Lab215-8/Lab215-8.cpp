// Lab215-8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFPhysics.h>
#include <vector>

using namespace std;
using namespace sf;
using namespace sfp;

void LoadTex(Texture& tex, string filename) {
    if (!tex.loadFromFile(filename)) {
        cout << "Could not load" << filename << endl;
        exit(1);
    }
}

int main()
{
    RenderWindow window(VideoMode(800, 600), "Duck Hunter");
    World world(Vector2f(0, 0));
    int score(0);
    int arrows(5);

    PhysicsSprite crossBow;
    Texture cbowTex;
    LoadTex(cbowTex, "images/crossbow.png");
    Vector2f sz = crossBow.getSize();
    crossBow.setTexture(cbowTex);
    crossBow.setCenter(Vector2f(400, 600 - (sz.y / 2)));

    PhysicsSprite arrow;
    Texture arrowTex;
    LoadTex(arrowTex, "images/arrow.png");
    arrow.setTexture(arrowTex);
    bool drawingArrow(false);

    PhysicsRectangle top;
    top.setSize(Vector2f(800, 10));
    top.setCenter(Vector2f(400, 5));
    top.setStatic(true);
    world.AddPhysicsBody(top);

    PhysicsRectangle left;
    left.setSize(Vector2f(10, 600));
    left.setCenter(Vector2f(5, 300));
    left.setStatic(true);
    world.AddPhysicsBody(left);


    Texture duckTex;
    LoadTex(duckTex, "images//duck.png");
    PhysicsShapeList<PhysicsSprite> ducks;
  


    top.onCollision = [&drawingArrow, &world, &arrow]
    (PhysicsBodyCollisionResult result) {
        drawingArrow = false;
        world.RemovePhysicsBody(arrow);
    };
    
    Font fnt;
    if (!fnt.loadFromFile("arial.ttf")) {
        cout << "Could not load font." << endl;
        exit(3);
    }


    Clock clock;
    Time lastTime(clock.getElapsedTime());
    Time currentTime(lastTime);
    long duckMS(0);
    while ((arrows > 0) || drawingArrow) {
        currentTime = clock.getElapsedTime();
        Time deltaTime = currentTime - lastTime;
        long deltaMS = deltaTime.asMilliseconds();
        if (deltaMS > 9) {
            duckMS = duckMS + deltaMS;
            lastTime = currentTime;
            world.UpdatePhysics(deltaMS);
            if (Keyboard::isKeyPressed(Keyboard::Space) && !drawingArrow) {
                drawingArrow = true;
                arrows = arrows - 1; 
                arrow.setCenter(crossBow.getCenter());
                arrow.setVelocity(Vector2f(0, -1));
                world.AddPhysicsBody(arrow);
            }
            if (duckMS > 2000) {
                duckMS = 0;
                PhysicsSprite& duck = ducks.Create();
                duck.setTexture(duckTex);
                int x = 800;
                Vector2f sz = duck.getSize();
                duck.setCenter(Vector2f(x, 20 + (sz.y / 2)));
                duck.setVelocity(Vector2f(-0.25, 0));
                world.AddPhysicsBody(duck);
                duck.onCollision =
                    [&drawingArrow, &world, &arrow, &duck, &ducks, &score, &left]
                    (PhysicsBodyCollisionResult result) {
                    if (result.object2 == arrow) {
                        drawingArrow = false;
                        world.RemovePhysicsBody(arrow);
                        world.RemovePhysicsBody(duck);
                        ducks.QueueRemove(duck);
                        score += 10;
                    }
                    if (result.object2 == left) {
                        world.RemovePhysicsBody(duck);
                        ducks.QueueRemove(duck);
                    }
                    }
                ;
            }


            window.clear();
            if (drawingArrow) {
                window.draw(arrow);
            }
            
            ducks.DoRemovals();
            for (auto duck : ducks) {
                window.draw(duck);
            }

            window.draw(crossBow);
            Text scoreText;
            scoreText.setString(to_string(score));
            scoreText.setFont(fnt);
            window.draw(scoreText);
            Text arrowCountText;
            arrowCountText.setString(to_string(arrows));
            arrowCountText.setFont(fnt);
            float countx = arrowCountText.getLocalBounds().width;
            float county = arrowCountText.getLocalBounds().height;
            arrowCountText.setPosition(790 - countx, 0);
            window.draw(arrowCountText);

            window.display();
           

            //world.VisualizeAllBounds(window);
           

        }
    }
    window.display();
    Text gameOverText;
    gameOverText.setString("GAME OVER");
    gameOverText.setFont(fnt);
    float gameOverx = gameOverText.getLocalBounds().width;
    float gameOvery = gameOverText.getLocalBounds().height;
    gameOverText.setPosition(400 - (gameOverx/2), 300 - gameOvery);
    window.draw(gameOverText);
    window.display();
    while (true);
    

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
