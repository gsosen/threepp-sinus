#include <vector>
#include <memory>
#include <cmath>

#include "threepp/threepp.hpp"
using namespace threepp;

int main() {
    Canvas canvas("threepp");
    GLRenderer renderer(canvas);

    auto scene = Scene::create();
    scene->background = Color::aliceblue;

    auto camera = PerspectiveCamera::create(60, canvas.aspect(), 0.1f, 1000);
    camera->position.z = 10;

    canvas.onWindowResize([&](WindowSize size) {
        camera->aspect = size.aspect();
        camera->updateProjectionMatrix();
        renderer.setSize(size);
    });

    auto light = DirectionalLight::create();
    light->position.set(1, 2, 3);
    scene->add(light);
    scene->add(AmbientLight::create(0xffffff, 0.4f));


    const int antall = 8;

    std::vector<std::shared_ptr<Mesh>> bokser;

    auto geometry = BoxGeometry::create(0.7, 0.7, 0.7);

    for (int i = 0; i < antall; i++) {
        auto material = MeshStandardMaterial::create();
        material->color = Color::blue;

        auto box = Mesh::create(geometry, material);

        box->position.x = i - (antall - 1) / 2.0f;

        scene->add(box);
        bokser.push_back(box);
    }

    int tastetrykk = 0;

    canvas.onKeyPressed([&](KeyEvent evt) {
        if (evt.key == Key::SPACE) {

            int gruppe = tastetrykk % 2;

            for (int i = 0; i < antall; i++) {
                if (i % 2 == gruppe) {
                    auto material = std::dynamic_pointer_cast<MeshStandardMaterial>(
                        bokser[i]->material()
                    );

                    if (material) {
                        if (tastetrykk % 4 < 2) {
                            material->color = Color::orange;
                        } else {
                            material->color = Color::blue;
                        }
                    }
                }
            }

            tastetrykk++;
        }
    });



    Clock clock;

    const float amplitude = 1.0f;
    const float frekvens = 0.5f;

    canvas.animate([&] {
        const float t = clock.getElapsedTime();

        for (int i = 0; i < antall; i++) {
            float fase = 0;

            if (i % 2 == 1) {
                fase = math::PI;
            }

            bokser[i]->position.y =
                amplitude * std::sin(2 * math::PI * frekvens * t + fase);
        }

        renderer.render(*scene, *camera);
    });


}
