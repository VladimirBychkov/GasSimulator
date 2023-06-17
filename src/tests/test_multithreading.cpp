#include <mutex>
#include <type_traits>
#include <thread>
#include <vector>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>


template<typename T>
class ThreadSafe {
private:
    T t;
    mutable std::mutex mtx;

public:
    template<typename... Args>
    ThreadSafe(Args&&... args) : t(std::forward<Args>(args)...) {}

    //ThreadSafe(const ThreadSafe& other) {
    //    std::lock_guard<std::mutex> lock(other.mtx);
    //    t = other.t;
    //    //mtx = other.mtx;
    //}
    //ThreadSafe& operator=(const ThreadSafe& other) {
    //    t = other.t;
    //    //mtx = other.mtx;
    //    return *this;
    //}

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
    //Particle() { position = 0; }
    Particle(double pos) : position(pos) {}
    //Particle(const Particle& other) {
    //    position = other.position;
    //}

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

int main() {
    using SafeParticle = ThreadSafe<Particle>;
    //SafeParticle particle1(0.0);
    //SafeParticle particle2(2.0);
    //
    //particle1.operate([](Particle& p) { p.setPosition(1.0); });
    //double position = particle1.operate([](Particle& p) { return p.getPosition(); });

    sf::Clock clock;
    std::vector<SafeParticle*> particles;
    SafeParticle particle(0.0);
    //std::vector<double> data;
    size_t num{ 1'000'000 };
    particles.reserve(num);
    //data.reserve(num);

    clock.restart();
    for (size_t i = 0; i < num; i++) {
        particles.push_back(new SafeParticle(static_cast<double>(i)));
    }
    std::cout << "Init: " << clock.getElapsedTime().asMicroseconds() << std::endl;
    
    //std::thread t2(read_worker<Particle>, num, std::ref(particles), std::ref(data));
    std::thread t1(swap_worker<SafeParticle*>, num, std::ref(particles));
    std::thread t2(swap_worker<SafeParticle*>, num, std::ref(particles));
    //swap_worker<SafeParticle*>(num, particles);
    
    t1.join();
    t2.join();
    
    //clock.restart();
    //for (size_t i = 0; i < num / 2; i++) {
    //    swapPositions(particles[i], particles[num - 1 - i]);
    //}
    //std::cout << "Swap: " << clock.getElapsedTime().asMicroseconds() << std::endl;
    //
    int counter{ 0 };
    for (size_t i = 0; i < num; i++) {
        if (particles[i]->getT().getPosition() != i) {
            counter++;
            std::cout << i << std::endl;
        }
    }
    std::cout << "Incorrectly swaped " << counter << std::endl;

	return 0;
}