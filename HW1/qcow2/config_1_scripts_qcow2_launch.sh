qemu-system-x86_64 -hda ubuntu.img -boot d -m 2046 -boot strict=on

#!/bin/bash

# Define CPU and memory configurations
cpus=(4 8)
mems=(4096 8192) # Memory in MB

# Define disk images paths
disk_images=("path/to/ubuntu.img")

# Define user and host for SSH
username="rpandey4"
hostname="localhost" # or the IP address of the QEMU guest
ssh_port="22" # Replace with your QEMU SSH port if not the default

# Path to the sysbench executable on the guest
sysbench_path="sysbench"

# Function to run QEMU with the specified disk image, CPUs, and memory
run_qemu() {
    disk_image=$1
    cpu_count=$2
    memory_size=$3

    # Start QEMU with the current configuration
    qemu-system-x86_64 -hda ubuntu.img -boot d -m $memory_size -smp cpu_count,maxcpus= cpu_count -boot strict=on

}

# Main loop to run the experiments
for cpu in "${cpus[@]}"; do
    for mem in "${mems[@]}"; do
        for img in "${disk_images[@]}"; do
            echo "Testing with $cpu CPUs, $mem MB memory, using disk image $img"
            result_file="result_cpu${cpu}mem${mem}$(basename $img .img).txt"
            
            # Run QEMU with the specified configuration
            run_qemu $img $cpu $mem
            
            # Wait for the QEMU VM to start and SSH to become available
	    echo "waiting for vm to become available"
            sleep 120 # Adjust based on your system's boot time

            # SSH to the QEMU VM and run the sysbench test
            sshpass -p 'rpandey4' ssh -o StrictHostKeyChecking=no -p $ssh_port $username@$hostname "$sysbench_path --test=cpu --cpu-max-prime=10000 run" > "$result_file"
            
            # Shutdown the QEMU VM
            sshpass -p 'rpandey4' ssh -o StrictHostKeyChecking=no -p $ssh_port $username@$hostname "sudo poweroff"

            # Wait for QEMU VM to shutdown
            sleep 10
        done
    done
done

echo "Testing completed."

