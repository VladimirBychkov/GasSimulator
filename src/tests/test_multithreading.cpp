#include <mutex>
#include <type_traits>
#include <thread>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>

#include "../multithreading.h"


template<typename T>
class ThreadSafe {
private:
    T t;
    mutable std::mutex mtx;

public:
    template<typename... Args>
    ThreadSafe(Args&&... args) : t(std::forward<Args>(args)...) {}

    std::mutex& getMutex() {
        return mtx;
    }

    T& getT() {
        return t;
    }

    template<typename Function>
    auto operate(Function func) -> decltype(func(t)) {
        std::lock_guard<std::mutex> lock(mtx);
        return func(t);
    }
};

class Particle {
private:
    double position;
public:
    Particle(double pos) : position(pos) {}

    double getPosition() const {
        return position;
    }

    void setPosition(double pos) {
        position = pos;
    }
};

template<typename T>
auto swapPositions(T& particle1, T& particle2) -> std::enable_if_t<std::is_same_v<T, ThreadSafe<Particle>>, void> {
    std::lock(particle1.getMutex(), particle2.getMutex());
    std::lock_guard<std::mutex> lock1(particle1.getMutex(), std::adopt_lock);
    std::lock_guard<std::mutex> lock2(particle2.getMutex(), std::adopt_lock);

    double temp = particle1.getT().getPosition();
    particle1.getT().setPosition(particle2.getT().getPosition());
    particle2.getT().setPosition(temp);
}

template<typename T>
auto swapPositions(T& particle1, T& particle2) -> std::enable_if_t<std::is_same_v<T, Particle>, void> {
    double temp = particle1.getPosition();
    particle1.setPosition(particle2.getPosition());
    particle2.setPosition(temp);
}

template<typename T>
void swap_worker(int num, std::vector<T>& particles) {
    for (size_t i = 0; i < num / 2; i++) {
        swapPositions(*particles[i], *particles[num - 1 - i]);
    }
}

template<typename T>
void read_worker(int num, std::vector<T>& particles, std::vector<double>& data) {
    for (size_t i = 0; i < num; i++) {
        data.push_back(particles[i].getPosition());
    }
}

//template<typename T>
void fill_worker(size_t i_start, size_t i_end, std::vector<float>& data) {
    for (size_t i = i_start; i < i_end; i++) {
        data[i] = static_cast<float>(i);
    }
}

int main() {
    using SafeParticle = ThreadSafe<Particle>;

    sf::Clock clock;

    /*
    std::vector<SafeParticle*> particles;
    SafeParticle particle(0.0);
    size_t num{ 1'000'000 };
    particles.reserve(num);

    clock.restart();
    for (size_t i = 0; i < num; i++) {
        particles.push_back(new SafeParticle(static_cast<double>(i)));
    }
    std::cout << "Init: " << clock.getElapsedTime().asMicroseconds() << std::endl;
    
    std::thread t1(swap_worker<SafeParticle*>, num, std::ref(particles));
    std::thread t2(swap_worker<SafeParticle*>, num, std::ref(particles));
    
    t1.join();
    t2.join();
    
    int counter{ 0 };
    for (size_t i = 0; i < num; i++) {
        if (particles[i]->getT().getPosition() != i) {
            counter++;
            std::cout << i << std::endl;
        }
    }
    std::cout << "Incorrectly swaped " << counter << std::endl;
    */

    std::vector<float> data;
    //data.reserve(10'000'000);
    data.resize(10);

    clock.restart();
    runInMultipleThreads(3, fill_worker, 0, 10, data);
    std::cout << "Time: " << clock.getElapsedTime().asMicroseconds() << std::endl;
    
    std::cout << data[2] << std::endl;


	return 0;
}