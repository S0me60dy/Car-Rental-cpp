#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <vector>
#include <ctime>
#include <memory>
#include "json.hpp"
#include "customer.hpp"
using std::string;
using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::find;
using std::find_if;
using std::begin;
using std::end;
using std::map;
using std::set;
using json = nlohmann::json;

class Rental{
    private:
    string number_plate;
    string customer_name;
    string start_rent;
    string end_rent;
    public:
        // Constructor
        Rental (string number_plate, string customer_name, string start_rent, string end_rent)
        : number_plate(number_plate), customer_name(customer_name), start_rent(start_rent), end_rent(end_rent) {}
        
        //Getters
        const string getNumberPlate() const { return number_plate; }
        const string getCustomerName() const {return customer_name;}
        const string getStartRent() const {return start_rent;}
        const string getEndRent() const {return end_rent;}

        json toJSON() const{
            return {
                {"number_plate", number_plate},
                {"customer_name", customer_name},
                {"start_rent", start_rent},
                {"end_rent", end_rent}
            };
        }

        // Static method to load rental from JSON
        static Rental fromJSON(const json& j) {
            return Rental(j["number_plate"], j["customer_name"], j["start_rent"], j["end_rent"]);
        }

        // Display rental details
        void display() const {
            cout << "Car: " << number_plate << ", Renter: " << customer_name
                << ", Rental Date: " << start_rent << ", Return Date: " << end_rent << endl;
        }
};

class RentalSystem{
    protected:
        map<string, string> rent_storage; // rent_storage['number_plate'] = renter;
        set<string> renters;
        vector<Rental> rents;
    public:
        // class constructor
        RentalSystem() = default;
        
        // func to show all "number_plate": "Customer_name" entries in the map rent_storage
        void displayRentals(){
             for (auto it = rent_storage.cbegin(); it != rent_storage.cend(); it++){
                cout << it->first << ": " << it->second << endl;
            };
        }


        void rentCar(vector<unique_ptr<Car>>& cars, 
                    vector<unique_ptr<Customer>>& customers, 
                    const string& customer_name, 
                    const string& car_number_plate, 
                    const string& start_rent, 
                    const string& end_rent){
            auto carIt = find_if(cars.begin(), cars.end(), [&car_number_plate](const unique_ptr<Car>& car) { return car->getNumberPlate() == car_number_plate;});
            
            if(carIt == cars.end()){
                cout << "Reached the end of the array without finding the car." << endl;
                return;
            }

            if (!(*carIt)->isAvailable()){
                cout << "Car is unavailable!" << endl;
                return;
            }

            auto customerIt = find_if(customers.begin(), customers.end(), [&customer_name](const unique_ptr<Customer>& customer) {return customer->getName() == customer_name;});

            if (customerIt == customers.end()) {
                cout << "Error 404. Customer not found!" << endl;
                return;
            }

            (*carIt)->setAvailability(false);
            rent_storage[car_number_plate] = customer_name;
            renters.insert(customer_name);
            rents.emplace_back(car_number_plate, customer_name, start_rent, end_rent);

            cout << "Car assigned successfully to " << customer_name << endl;
        }

        void returnCar(vector<unique_ptr<Car>>& cars, const string& car_number_plate) {
            auto it = rent_storage.find(car_number_plate);
            if (it != rent_storage.end()) {
                auto carIt = find_if(cars.begin(), cars.end(),
                    [&car_number_plate](const unique_ptr<Car>& car) {
                        return car->getNumberPlate() == car_number_plate;
                    });

                if (carIt != cars.end()) {
                    (*carIt)->setAvailability(true);
                    cout << "Car with number plate " << car_number_plate << " has been returned.\n";
                }

                renters.erase(it->second);  // Remove renter name
                rent_storage.erase(it);    // Remove car from rented list
            } else {
                cout << "Car with number plate " << car_number_plate << " was not rented.\n";
            }
        }

        void loadRentals(string& rent_file){
            ifstream input_stream(rent_file);

            if(!input_stream){
                cout << "Could not locate rent file..." << endl;
                return;
            }
            

            json rentData;
            input_stream >> rentData;
            input_stream.close();

            if (rentData.empty()) {
                cout << "Rent data is empty. No rentals loaded.\n";
                return;
            }

            for (const auto& item : rentData){
                rents.push_back(Rental::fromJSON(item));
                rent_storage[item["number_plate"]] = item["customer_name"];
                renters.insert(item["customer_name"]);
            }

            cout << "Rents loaded successfully" << endl;
        }

        void saveRentals(string& rent_file){
            ofstream output_stream(rent_file);

            json rentData;
            for (const auto& rent : rents) {
                json rentalEntry;
                rentalEntry["number_plate"] = rent.getNumberPlate();
                rentalEntry["customer_name"] = rent.getCustomerName();
                rentalEntry["start_rent"] = rent.getStartRent();
                rentalEntry["end_rent"] = rent.getEndRent();
                rentData.push_back(rentalEntry);
            }
            if (output_stream){
                output_stream << rentData.dump(4);
                cout << "Rent data saved successfully!" << endl;
            } else {
                cout << "Error loading data in the file!" << endl; 
            }
        }
};