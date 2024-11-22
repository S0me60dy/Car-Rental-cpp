#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "json.hpp"
#include "car.hpp"
using std::string;
using std::cout;
using std::cin;
using std::ifstream;
using std::ofstream;
using std::endl;
using std::vector;
using json = nlohmann::json;
class Rental{
    unsigned short int customer_id;
    string number_plate;
    time_t start_rent;
    time_t end_rent;
};

void modify_car(string cars_file, vector<Car*>& cars){
    ifstream input_stream(cars_file);
    if (!input_stream){
        cout << "Error: Unable to open the file!" << endl;
        return;
    }
    json carsData;
    input_stream >> carsData;
    input_stream.close();

    string number_plate;
    cout << "Please choose number plate of a car to modify: ";
    cin >> number_plate;

    bool car_found = false;


    for (auto& car : carsData["cars"]){
        if (car["number_plate"] == number_plate){
            car_found = true;
            int choice;
            cout << "What do you want to modify?" << endl;
            cout << "1. Brand" << endl;
            cout << "2. Model" << endl;
            cout << "3. Year" << endl;
            cout << "4. Availability" << endl;
            cout << "5. Daily Price" << endl;

            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: {
                    string new_brand;
                    cout << "Enter new brand: ";
                    cin >> new_brand;
                    car["brand"] = new_brand;
                    break;
                }
                case 2: {
                    string new_model;
                    cout << "Enter new model: ";
                    cin >> new_model;
                    car["model"] = new_model;
                    break;
                }
                case 3: {
                    int new_year;
                    cout << "Enter new year: ";
                    cin >> new_year;
                    car["year"] = new_year;
                    break;
                }
                case 4: {
                    bool new_availability;
                    cout << "Enter availability (1 for true, 0 for false): ";
                    cin >> new_availability;
                    car["available"] = new_availability;
                    break;
                }
                case 5: {
                    double new_price;
                    cout << "Enter new daily price: ";
                    cin >> new_price;
                    car["day_price"] = new_price;
                    break;
                }
                default:
                    cout << "Invalid choice!" << endl;
                    return;
            }

            cout << "Car details updated successfully!" << endl;
            break;
        }
    }
    if (!car_found){
        cout << "Invalid number plate!";
    }
    
    ofstream output_file(cars_file);
    if (output_file){
        output_file << carsData.dump(4);
        cout << "Changes saved to the file successfully!" << endl;
    } else {
        cout << "Cannot write changes to the file!" << endl;
    }
}

void remove_car(string cars_file, vector<Car*>& cars) {
    ifstream input_stream(cars_file);

    if (!input_stream) {
        cout << "Error: Unable to open the file!" << endl;
        return;
    }

    json carsData;
    input_stream >> carsData;
    input_stream.close();

    string delete_record;
    cout << "Please enter the number plate of the car to delete: ";
    cin >> delete_record;

    bool found = false;

    // Create a new array excluding the car to delete
    json updatedCars = json::array();
    for (const auto& item : carsData["cars"]) {
        if (item["number_plate"].get<std::string>() == delete_record) {
            found = true; // Mark that we've found the car to delete
        } else {
            updatedCars.push_back(item); // Keep the other cars
        }
    }

    if (!found) {
        cout << "No car found with the given number plate." << endl;
        return;
    }

    // Update JSON and write back to the file
    carsData["cars"] = updatedCars;
    ofstream output_stream(cars_file);
    if (output_stream) {
        output_stream << carsData.dump(4);
        cout << "Car with number plate " << delete_record << " has been removed successfully!" << endl;
    } else {
        cout << "Error: Unable to write to the file!" << endl;
    }
}

void add_car(string cars_file, vector<Car*>& cars ){
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

    Car* car = nullptr;
    if (electric == 'y' or electric == 'Y') {
        cout << "Kms: ";
        cin >> kms;
        cout << endl;
        newCar["type"] = "ElectricCar";
        newCar["kilometers"] = kms;

        car = new ElectricCar(number_plate, brand, model, year, available, day_price, kms);

    } else {
        newCar["type"] = "Car";
        car = new Car(number_plate, brand, model, year, available, day_price);
    }

    cars.push_back(car);

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
        cout << "New car entry added: ";
        cout << car << endl;
    } else {
        cout << "Unable to locate the file to write the new entry!" << endl;
    }
}

int menu_admin(){
    int choice;
    cout << "1. Rented Cars"<< endl; 
	cout << "2. New Car"<< endl;
	cout << "3. Modify Car"<< endl;
	cout << "4. Remove Car"<< endl;
	cout << "5. Show All Cars"<< endl;
	cout << "6. Change Rent Records"<< endl;
	cout << "7. Show Rent Records"<< endl;
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
    cout << "3. Show My History" << endl;
    cout << "0. Exit" << endl;
    cout << "--- Choose any one option ---" << endl;
	cout << "Enter one option: ";
    cin >> choice;
    return choice;
}

void display_cars(const vector<Car*>& cars){
    for (auto& car : cars){
        car->display();
    }
}

void car_records(string cars_file, vector<Car*>& cars){
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
            cars.push_back(new Car(item["number_plate"], item["brand"], item["model"], item["year"], item["available"], item["day_price"]));
        } else if (item["type"] == "ElectricCar") {
            cars.push_back(new ElectricCar(item["number_plate"], item["brand"], item["model"], item["year"], item["available"], item["day_price"], item["kilometers"]));
        }
    }
}

unsigned char home(){
    unsigned char mode;
    cout << "Please choose a mode (a-admin, c-customer): ";
    cin >> mode;
    return mode;
}


int main() {
    int something;
    string cars_file = "D:\\Studies\\c++\\cars.json";
    string rent_file = "D:\\Studies\\c++\\rent.json";
    
    // initialize a polymorphic vector to store all the cars
    vector<Car*> cars;

    car_records(cars_file, cars);
    

    unsigned char mode = home();

    
    

    while(true){
        something = menu_admin();
        switch (something) {
            case 1:
                cout << "Number 1 was chosen" << endl;
                break;
            case 2:
                cout << "You chose to add new car!" << endl;
                add_car(cars_file, cars);
                break;
            case 3:
                cout << "You chose to modify existing cars!" << endl;
                display_cars(cars);
                modify_car(cars_file, cars);
                break;
            case 4:
                cout << "You chose to remove a car!" << endl;
                cout << "Existing cars: " << endl;
                display_cars(cars);
                remove_car(cars_file, cars);
                break;
            case 5:
                display_cars(cars);
                break;
            case 0:
                cout << "Thank you for using our car rental software! See ya:)" << endl;
                for (auto& car : cars) {
                    delete car;
                }
                return 0;

            default:
                cout << "Please, enter a valid choice! Don't play with this software, please:(" << endl;
                break;
        }
        
        
    }
    
}


