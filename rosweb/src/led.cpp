#include <iostream>
#include <string>
#include <curl/curl.h>
#include <ros/ros.h>

// This function will be used to write the response data to a string
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main(int argc, char** argv) {
    // Initialize the ROS node
    ros::init(argc, argv, "sensor_value_node");
    ros::NodeHandle nh;

    CURL* curl;
    CURLcode res;
    std::string readBuffer;
    
    ros::Rate loop_rate(1); // Set the loop rate (e.g., 1 Hz)

    while (ros::ok()) {
        readBuffer.clear(); // Clear the buffer to store new response

        curl = curl_easy_init();
        if(curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.1.184/sensor");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            
            // Perform the request
            res = curl_easy_perform(curl);
            
            // Check for errors
            if(res != CURLE_OK) {
                ROS_ERROR("curl_easy_perform() failed: %s", curl_easy_strerror(res));
            } else {
                // Output the response using ROS logging
                ROS_INFO("Response: %s", readBuffer.c_str());
            }
            
            // Cleanup
            curl_easy_cleanup(curl);
        }
    std::string response = readBuffer.c_str();
	int extractedInt;
	std::stringstream ss(response); // Create a stringstream object from the response
	ss >> extractedInt; // Extract the integer
        
	if (extractedInt == 0){
	ROS_INFO("0");}
        if (extractedInt == 1){
        ROS_INFO("1");}
        if (extractedInt == 2){
        ROS_INFO("2");}
        if (extractedInt == 3){
        ROS_INFO("3");}
	
	ros::spinOnce(); // Handle ROS events
        loop_rate.sleep(); // Sleep for the remainder of the loop cycle
    
    
    }

    return 0;
}
