#include <iostream>
#include <map>
#include <set>
#include <fstream>
#include <vector>
#include <ctime>
#include <memory>
#include "json.hpp"
#include "car.hpp"
#include "rent.hpp"
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
using std::begin;
using std::end;
using std::map;
using std::set;
using json = nlohmann::json;


void modify_car(string cars_file, vector<unique_ptr<Car>>& cars) {
    ifstream input_stream(cars_file);
    if (!input_stream) {
        cout << "Error: Unable to open the file!" << endl;
        return;
    }

    // Read the existing data from the JSON file
    json carsData;
    input_stream >> carsData;
    input_stream.close();

    // Prompt the user for the number plate of the car they wish to modify
    string number_plate;
    cout << "Please enter the number plate of the car to modify: ";
    cin >> number_plate;

    // Iterate over the cars vector and search for the car with the specified number plate
    unique_ptr<Car>* car_to_modify = nullptr;
    for (auto& car : cars) {
        if (car->getNumberPlate() == number_plate) {
            car_to_modify = &car;  
            break;
        }
    }

    // Check if the car was found
    if (car_to_modify == nullptr) {
        cout << "Error: Car with number plate " << number_plate << " not found!" << endl;
        return;
    }

    int choice = -1;
    while (choice != 0) {
        // Display modification menu
        cout << "What would you like to modify?\n"
             << "1. Brand\n"
             << "2. Model\n"
             << "3. Year\n"
             << "4. Availability\n"
             << "5. Daily Price\n"
             << "0. Exit\n"
             << "Enter your choice: ";
        cin >> choice;

        // Handle the user's choice
        switch (choice) {
            case 1: {
                string new_brand;
                cout << "Enter new brand: ";
                cin >> new_brand;
                (*car_to_modify)->setBrand(new_brand);
                break;
            }
            case 2: {
                string new_model;
                cout << "Enter new model: ";
                cin >> new_model;
                (*car_to_modify)->setModel(new_model);
                break;
            }
            case 3: {
                int new_year;
                cout << "Enter new year: ";
                cin >> new_year;
                (*car_to_modify)->setYear(new_year);
                break;
            }
            case 4: {
                bool new_availability;
                cout << "Enter availability (1 for available, 0 for not available): ";
                cin >> new_availability;
                (*car_to_modify)->setAvailability(new_availability);
                break;
            }
            case 5: {
                double new_price;
                cout << "Enter new daily price: ";
                cin >> new_price;
                (*car_to_modify)->setDayPrice(new_price);
                break;
            }
            case 0:
                cout << "Exiting modification menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    // update the JSON file
    for (auto& car_data : carsData["cars"]) {
        if (car_data["number_plate"] == number_plate) {
            car_data["brand"] = (*car_to_modify)->getBrand();
            car_data["model"] = (*car_to_modify)->getModel();
            car_data["year"] = (*car_to_modify)->getYear();
            car_data["available"] = (*car_to_modify)->isAvailable();
            car_data["day_price"] = (*car_to_modify)->getDayPrice();
            break; 
        }
    }

    ofstream output_file(cars_file);
    if (output_file) {
        output_file << carsData.dump(4);
        cout << "Changes saved to the file successfully!" << endl;
    } else {
        cout << "Cannot write changes to the file!" << endl;
    }
}
// update the function for unique_ptr use
void remove_car(string cars_file, vector<unique_ptr<Car>>& cars) {
    // Open the JSON file to read the current cars data
    ifstream input_stream(cars_file);
    if (!input_stream) {
        cout << "Error: Unable to open the file!" << endl;
        return;
    }

    json carsData;
    input_stream >> carsData;  // Read the existing data from the file
    input_stream.close();

    // Ask the user for the number plate of the car they want to remove
    string number_plate;
    cout << "Please enter the number plate of the car to remove: ";
    cin >> number_plate;

    // Search for the car in the vector using for loop
    auto it = cars.begin();
    while (it != cars.end()) {
        if ((*it)->getNumberPlate() == number_plate) {
            it = cars.erase(it); 
            cout << "Car with number plate " << number_plate << " has been removed." << endl;
            break;
        } else {
            ++it;
        }
    }


    if (it == cars.end()) {
        cout << "Error: Car with number plate " << number_plate << " not found!" << endl;
        return;
    }

    // Update JSON
    bool car_removed = false;
    for (auto it = carsData["cars"].begin(); it != carsData["cars"].end(); ++it) {
        if ((*it)["number_plate"] == number_plate) {
            it = carsData["cars"].erase(it);
            car_removed = true;
            break;
        }
    }

    if (!car_removed) {
        cout << "Error: Car with number plate " << number_plate << " not found in the data file!" << endl;
        return;
    }

    ofstream output_file(cars_file);
    if (output_file) {
        output_file << carsData.dump(4);
        cout << "Changes saved to the file successfully!" << endl;
    } else {
        cout << "Cannot write changes to the file!" << endl;
    }
}

void add_car(string cars_file, vector<unique_ptr<Car>>& cars ){
    unsigned char electric;
    string number_plate, brand, model;
    int year;
    bool available;
    double day_price;
    int kms;
    cout << "Type 'y' if the car is e-car: ";
    cin >> electric;
    cout << "number plate: ";
    cin >> number_plate;
    cout << endl;
    cout << "brand: ";
    cin >> brand;
    cout << endl;
    cout << "model: ";
    cin >> model;
    cout << endl;
    cout << "year: ";
    cin >> year;
    cout << endl;
    cout << "available: ";
    cin >> available;
    cout << endl;
    cout << "Daily price: ";
    cin >> day_price;
    cout << endl;
    json newCar;
    newCar["number_plate"] = number_plate;
    newCar["brand"] = brand;
    newCar["model"] = model;
    newCar["year"] = year;
    newCar["available"] = available;
    newCar["day_price"] = day_price;
    if (electric == 'y' or electric == 'Y') {
        cout << "Kms: ";
        cin >> kms;
        cout << endl;
        cars.push_back(make_unique<ElectricCar>(number_plate, brand, model, year, available, day_price, kms));
        newCar["type"] = "ElectricCar";
        newCar["kilometers"] = kms;
    } else {
        cars.push_back(make_unique<Car>(number_plate, brand, model, year, available, day_price));

        newCar["type"] = "Car";
    }

    json carsData;
    ifstream input_file(cars_file);
    if (input_file){
        input_file >> carsData;
    }
    input_file.close();


    carsData["cars"].push_back(newCar);


    ofstream output_file(cars_file);
    if (output_file){
        output_file << carsData.dump(4);
        cout << "New car entry added!";
    } else {
        cout << "Unable to locate the file to write the new entry!" << endl;
    }
}

int menu_admin(){
    int choice;
    cout << "1. Show Rent Records"<< endl; 
	cout << "2. New Car"<< endl;
	cout << "3. Modify Car"<< endl;
	cout << "4. Remove Car"<< endl;
	cout << "5. Show All Cars"<< endl;
	cout << "6. Save Rent Records"<< endl;
	cout << "0. Exit"<< endl;
	cout << "--- Choose any one option ---" << endl;
	cout << "Enter one option: ";
    cin >> choice;
    return choice;
}

int menu_customer(){
    int choice;
    cout << "1. Rent a Car" << endl;
    cout << "2. Return a Car" << endl;
    cout << "0. Exit" << endl;
    cout << "--- Choose any one option ---" << endl;
	cout << "Enter one option: ";
    cin >> choice;
    return choice;
}

void display_cars(const vector<unique_ptr<Car>>& cars){
    for (auto& car : cars){
        car->display();
    }
}

void car_records(string cars_file, vector<unique_ptr<Car>>& cars){
    // create an input file stream object
    ifstream input_stream(cars_file);
    
    // check if file exists
    if(!input_stream){
        cout << "Impossible to open the file." << endl;
        // stop executing if error
        return;
    }

    // parse json data (the input data is read directly to the json object)
    json carsData;
    input_stream >> carsData;

    /* 
    'const' to prevent modification of an object inside the loop, 
    tell compiler to deduce type of item based on the initializer(json) using 'auto',
    use addresses(&) to prevent copying
    */
    for (const auto& item : carsData["cars"]){
        if (item["type"] == "Car"){
            cars.push_back(make_unique<Car>(item["number_plate"], item["brand"], item["model"], item["year"], item["available"], item["day_price"]));
        } else if (item["type"] == "ElectricCar") {
            cars.push_back(make_unique<ElectricCar>(item["number_plate"], item["brand"], item["model"], item["year"], item["available"], item["day_price"], item["kilometers"]));
        }
    }
}

void customer_records(string customers_file, vector<unique_ptr<Customer>>& customers){
    ifstream input_stream(customers_file);

    if(!input_stream){
        cout << "Error opening customers file!" << endl;
        return;
    }

    json customersData;
    input_stream >> customersData;
    input_stream.close();

    for (const auto& item : customersData){
        customers.push_back(make_unique<Customer>(item["name"], item["email"], item["phone"]));
    } 
}


unsigned char home(){
    unsigned char mode;
    cout << "Please choose a mode (a-admin, c-customer): ";
    cin >> mode;
    return mode;
}


int main() {
    string cars_file = "D:\\Studies\\c++\\Project data\\cars.json";
    string rent_file = "D:\\Studies\\c++\\Project data\\rent.json";
    string customers_file = "D:\\Studies\\c++\\Project data\\customers.json";

    // Load cars from the file
    vector<unique_ptr<Car>> cars;
    car_records(cars_file, cars);

    // Load Customers from the file
    vector<unique_ptr<Customer>> customers;
    customer_records(customers_file, customers);

    RentalSystem rentalSystem;

    // Load rentals from the file
    rentalSystem.loadRentals(rent_file);
    
    unsigned char mode = home();

    while (true) {
        if (mode == 'a' || mode == 'A') {
            int adminChoice = menu_admin();
            switch (adminChoice) {
                case 1:
                    rentalSystem.displayRentals();
                    break;
                case 2:
                    cout << "Adding a new car...\n";
                    add_car(cars_file, cars);
                    break;
                case 3:
                    cout << "Modifying an existing car...\n";
                    modify_car(cars_file, cars);
                    break;
                case 4:
                    cout << "Removing a car...\n";
                    remove_car(cars_file, cars);
                    break;
                case 5:
                    display_cars(cars);
                    break;
                case 6:
                    rentalSystem.saveRentals(rent_file);
                    break;
                case 0:
                    cout << "Exiting...\n";
                    return 0;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } else if (mode == 'c' || mode == 'C') {
            string customerName, carNumberPlate, startRent, endRent;
            int customerChoice = menu_customer();
            switch (customerChoice) {
                case 1:
                    cout << "Enter your name: ";
                    cin >> customerName;
                    cout << "Enter car number plate: ";
                    cin >> carNumberPlate;
                    cout << "Enter start date (YYYY-MM-DD): ";
                    cin >> startRent;
                    cout << "Enter end date (YYYY-MM-DD): ";
                    cin >> endRent;
                    rentalSystem.rentCar(cars, customers, customerName, carNumberPlate, startRent, endRent);
                    rentalSystem.saveRentals(rent_file);
                    break;
                case 2:
                    cout << "Enter car number plate to return: ";
                    cin >> carNumberPlate;
                    rentalSystem.returnCar(cars, carNumberPlate);
                    rentalSystem.saveRentals(rent_file);
                    break;
                case 0:
                    cout << "Exiting customer mode...\n";
                    return 0;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        } else {
            cout << "Invalid mode. Please choose again.\n";
        }
    }
    return 0;
    
}


