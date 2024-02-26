import time
import requests

#Test cases sent to chatBot function and measure output of response times
def send_request(input_data):
    start_time = time.time()
    response = requests.post("http://10.62.0.4:8080/function/myChatBot", data=input_data)
    end_time = time.time()
    response_time = end_time - start_time
    return response, response_time

# Scenario 1: Response Time for the First Request without Figlet
input_data = "What's your name?"
response, response_time = send_request(input_data)
print("Scenario 1 - Response Time for First Request without Figlet:")
print("Server Response:", response.text)
print("Response Time:", response_time, "seconds")
print()

# Scenario 2: Response Time for the Second Request without Figlet
input_data = "What time is it now?"
response, response_time = send_request(input_data)
print("Scenario 2 - Response Time for Second Request without Figlet:")
print("Server Response:", response.text)
print("Response Time:", response_time, "seconds")
print()

# Scenario 3: Average Response Time Over 10 Requests without Figlet
total_response_time = 0
for _ in range(10):
    input_data = "What is your name?"
    _, response_time = send_request(input_data)
    total_response_time += response_time
average_response_time = total_response_time / 10
print("Scenario 3: Average Response Time Over 10 Requests without Figlet:")
print("Average Response Time:", average_response_time, "seconds")
print()

# Scenario 4: Response Time for the First Request with Figlet
input_data = "figlet for hello"
response, response_time = send_request(input_data)
print("Scenario 4 - Response Time for First Request with Figlet:")
print("Server Response:", response.text)
print("Response Time:", response_time, "seconds")
print()

# Scenario 5: Response Time for the Second Request with Figlet
input_data = "figlet for hello"
_, _ = send_request(input_data)  # Warm-up request
response, response_time = send_request(input_data)
print("Scenario 5 - Response Time for Second Request with Figlet:")
print("Server Response:", response.text)
print("Response Time:", response_time, "seconds")
print()

# Scenario 6: Response Time for Second Request with Figlet after a Request without Figlet
input_data = "What's your name?"  # Request without figlet
_, _ = send_request(input_data)  # Warm-up request
input_data = "figlet for hello"  # Request with figlet
response, response_time = send_request(input_data)
print("Scenario 6 - Response Time for Second Request with Figlet following a Request without Figlet:")
print("Server Response:", response.text)
print("Response Time:", response_time, "seconds")
print()

# Scenario 7: Average Response Time Over 10 Requests with Figlet
total_response_time = 0
for _ in range(10):
    input_data = "figlet for hello"
    _, response_time = send_request(input_data)
    total_response_time += response_time
average_response_time = total_response_time / 10
print("Scenario 7 - Average Response Time Over 10 Requests with Figlet:")
print("Average Response Time:", average_response_time, "seconds")

    
# def send_request(input_data):
# 	start_time = time.time()
# 	response = requests.post("http://10.62.0.4:8080/function/myChatBot", data=input_data)
# 	end_time = time.time()
# 	response_time = end_time - start_time
# 	return response, response_time

# # Test Scenario 1: Response Time for the First Request that Does Not Call Figlet
# input_data = "What is your name?"
# response, response_time = send_request(input_data)
# print("Test scenario 1 - Response Time for the First Request that Does Not Call Figlet:")
# print ("Response:", response.text) 
# print("Response Time:", response_time, "seconds")
# print()

# # Test Scenario 2: Response Time for the Second Request that Does Not Call Figlet
# input_data = "What time is it?"
# response, response_time = send_request(input_data)
# print("Test Scenario 2 - Response Time for the Second Request that Does Not Call Figlet:")
# print("Response:" response.text)
# print("Response Time:" , response_time, "seconds")
# print()

# # Test Scenario 3: Average Response Time Over 10 Requests that Do Not Call Figlet
# total_response_time = 0
# 	for _ in range(10):
# 	input data = "What is vour name?"
# 	_ , response_time = send_request(input_data)
# 	total_response_time += response_time
# average_ response time = total response time / 10
# print( “Test scenario 3: Average Response ime Over 10 Requests that Do Not Call Figlet:”)
# print("Average Response Time:", average_response_time, “seconds”)
# print()

# # Test Scenario 4: Response Time for the First Request that Calls Figlet
# input_data = "figlet for hello”
# response, response_time = send_request (input_data)
# print("Test Scenario 4 - Response Time for the First request that calls Figlet:")
# print("Response:”, response.text)
# print ("Response Time:", response_time, "seconds")
# print()

# # Test Scenario 5: Response Time for the Second Request that Calls Figlet
# input_data = "figlet for hello"
# _, _ = send_request(input _data) # First request to warm up the function response
# response, response_time = send_request(input_data)
# print(“Test Scenario 5 - Response Time for the Second Request that Calls Figlet:") 
# print("Response:" , response. text print("Response Time: , response_time, “seconds")
# print()

# # Test Scenario 6: Response Time for the Second Request that Calls Figlet Following the First Request that Does Not Call Figlet

# input data = "What is your name?" # First request that does not call figlet
# _’ _ = send_request(input_data) # Warm up the function
# input data = “fillet for hello" # Second request that calls figlet
# response, response time = send_request(input_data)
# print("Test Scenario 6 - Response Time for the Second Request that Calls Figlet Following the First Request that Does Not Call Figlet:")
# print("Response:" response.text)
# print( "Response Time:" , response_time, "seconds")
# print()

# # Test Scenario 7: Average Response Time Over 10 Requests that Call Figlet
# total response time = 0
# for _ in range (10):
# 	input _data = "figlet for hello"
# 	_, response_time = send_request (input_data)
# 	total_response_time += response_time
# average response time = total response time / 10
# print("Test Scenario 7- Average Response Time Over 10 Requests that Call Figlet:")
# print("Average Response Time: average_response_time, "seconds")