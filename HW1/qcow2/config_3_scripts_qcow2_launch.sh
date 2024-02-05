
#!/bin/bash

# Define CPU and memory configurations
cpus=(4)
mems=(8192) # Memory in MB

qemu-system-x86_64 -hda ubuntu.img -boot d -m $mems -smp cpus,maxcpus= cpus -boot strict=on


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

# Run QEMU with the specified configuration
run_qemu $disk_images $cpus $mems
   

echo "Testing completed..."