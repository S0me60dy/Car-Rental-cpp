#include <iostream>
#include "json.hpp"
using std::string;
using std::cout;
using std::cin;
using std::endl;
using json = nlohmann::json;

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
        
        //Getters

        /* "const" after the function name means that the function is a member function.
            Used to prevend modification cause we are using only get methods, we don't need to update anything.
        */

        const string& getNumberPlate() const { return number_plate; }
        const string& getBrand() const { return brand; }
        const string& getModel() const { return model; }
        int getYear() const { return year; }
        bool isAvailable() const { return available; }
        double getDayPrice() const { return day_price; }

        //Setters
        void setBrand(const string& brand_updated) {brand = brand_updated; }
        void setModel(const string& model_updated) {model = model_updated; }
        void setYear(const int& year_updated) {year = year_updated; }
        void setAvailability(const bool& available_updated) {available = available_updated; }
        void setDayPrice(const double& day_price_updated) {day_price = day_price_updated; }

        //JSON Serialization
        virtual json toJSON() const {
            return {
                {"type", "Car"},
                {"number_plate", number_plate},
                {"brand", brand},
                {"model", model},
                {"year", year},
                {"available", available},
                {"day_price", day_price}
            };
        }
};
class ElectricCar : public Car {
private:
    int kilometers; 

public:
    // Constructor
    ElectricCar(string np, string b, string m, int year, bool a, double dp, int kms)
        : Car(np, b, m, year, a, dp), kilometers(kms) {}

    // Override display method to include kilometers
    void display() const override {
        Car::display(); 
        cout << "Kilometres: " << kilometers << endl; 
    }

    // JSON Serialization (Override for ElectricCar)
    json toJSON() const override {
        json baseJson = Car::toJSON(); 
        baseJson["type"] = "ElectricCar";
        baseJson["kilometers"] = kilometers;
        return baseJson;
    }

    // Getter for kilometers
    int getKilometers() const { return kilometers; }

    // Setter for kilometers
    void setKilometers(int kms) { kilometers = kms; }
};