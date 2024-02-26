import requests
import threading
import time
import argparse

ADMIN_USER = "admin"
ADMIN_PASS = "PT5Ype4FzYj1MojIRYCJ7biGeLTrsk3CDtKD4X28JpMeyyLzUFxnZebI5cSw1sH"
DEFAULT_RATE = 10
DEFAULT_DURATION = 60
MAX_REPLICAS = 10

def send_request_to_bot(input_data):
    url = f"http://{ADMIN_USER}:{ADMIN_PASS}@10.62.0.4:8080/function/my-assistant"
    response = requests.post(url, data=input_data)
    return response

def fetch_current_bot_replica_count():
    url = f"http://{ADMIN_USER}:{ADMIN_PASS}@10.62.0.4:8080/system/functions"
    response = requests.get(url)
    functions = response.json()
    for function in functions:
        if function["name"] == "my-assistant":
            return function["replicas"]
    return 0

def adjust_bot_replicas(replicas):
    url = f"http://{ADMIN_USER}:{ADMIN_PASS}@10.62.0.4:8080/system/scale-function/my-assistant"
    data = {"replicas": replicas}
    requests.post(url, json=data)

def send_parallel_bot_requests(input_data, rate=DEFAULT_RATE, duration=DEFAULT_DURATION):
    start_time = time.time()
    end_time = start_time + duration
    request_count = 0
    
    while time.time() < end_time:
        start_time_request = time.time()
        threads = []

        for _ in range(rate):
            thread = threading.Thread(target=send_request_to_bot, args=(input_data,))
            thread.start()
            threads.append(thread)

        for thread in threads:
            thread.join()
            request_count += 1

        time_request = time.time() - start_time_request
    
    # Ensure rate of requests
    served_requests = request_count / duration
    current_replicas = fetch_current_bot_replica_count()
    print(f"Current replica count: {current_replicas}, Served requests: {served_requests}")
    
    # Scale replicas
    adjust_bot_replicas(current_replicas + 1)
    print(f"Current replica count after scaling: {current_replicas + 1}")

    with open("bot_scaling_log.txt", "a") as log_file:
        log_file.write(f"Current replica count: {current_replicas}, Served requests: {served_requests}\n")
        elapsed_time = time.time() - start_time
        total_requests = rate * duration
        queries_per_second = total_requests / elapsed_time
        log_file.write(f"Queries Per Second: {queries_per_second:.2f}\n")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Send parallel requests to the AI assistant function")
    parser.add_argument("input_data", type=str, help="Input data for the requests")
    parser.add_argument("--rate", type=int, default=DEFAULT_RATE, help="Rate of requests per second")
    parser.add_argument("--duration", type=int, default=DEFAULT_DURATION, help="Duration of the test in seconds")
    args = parser.parse_args()

    input_data = args.input_data
    rate = args.rate
    duration = args.duration

    send_parallel_bot_requests(input_data, rate, duration)
