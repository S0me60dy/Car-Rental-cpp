#include <iostream>
using std::string;
using std::cout;
using std::cin;
using std::endl;

class Car {
    // encapsulation used here
    protected:
        string number_plate;
        string brand;
        string model;
        int year;
        bool available;
        double day_price;
    public:
        // class constructor
        Car(string number_plate, string brand, string model, int year, bool available, double day_price)
        : number_plate(number_plate), brand(brand), model(model), year(year), available(available), day_price(day_price) {}
        // use of virtual function to "allow" polymorphism
        virtual void display() const {
        cout << "Car: " << number_plate << ", " << brand << " " << model << ", Year: " << year
             << ", Available: " << available << ", Day Price: $" << day_price << endl;
        }
        // definition of an automatic virtual destructor to prevent leaks
        virtual ~Car() = default;
        
        void checkAvailability(){
            if (this->available){
                cout << "The " << this->brand << " " << this->model << " is available" << endl;
            } else{
                cout << "Not available " << this->brand + " " + this->model << endl;
            }
        }
};
class ElectricCar: public Car{
    public:
        ElectricCar(string np, string b, string m, int year, bool a, double dp, int kms)
        : Car(np, b, m, year, a, dp), kilometers(kms) {}
        int kilometers;
        void display() const override{
            Car::display();
            cout << "Kilometres: " << this->kilometers << endl;
        }
};