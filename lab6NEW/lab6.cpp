#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

namespace mySpace
{    //alloc and free
	int has_initialized = 0;
	void *managed_memory_start;
	void *last_valid_address;

	//swapping
	int has_initialized_swap = 0;
	int  swap_file;
	int swap_used;
	int swap_size = 1024;
	void* swap_start;
	void* swap_end;

	void const *invalid_memory = reinterpret_cast<void *>(-1);

	auto malloc_init()
	{
		last_valid_address = sbrk(0);
		managed_memory_start = last_valid_address;
		has_initialized = 1;
		if (managed_memory_start == invalid_memory || last_valid_address == invalid_memory)
		{
			std::cout << "Memory init fail\n";
			return false;
		}
		return true;
	}

	struct mem_control_block
	{
		int is_available;
		int size;
	};

	void free(void *firstbyte)
	{
		struct mem_control_block *mcb;

		mcb = reinterpret_cast<mem_control_block *>(firstbyte - sizeof(struct mem_control_block));

		mcb->is_available = 1;

		return;
	}

	void *malloc(long numbytes)
	{
		void *current_location;
		struct mem_control_block *current_location_mcb;

		void *memory_location;
		if (!has_initialized)
		{
			malloc_init();
		}

		numbytes = numbytes + sizeof(struct mem_control_block);

		memory_location = 0;

		current_location = managed_memory_start;

		while (current_location != last_valid_address)
		{

			current_location_mcb = (struct mem_control_block *)current_location;

			if (current_location_mcb->is_available)
			{
				if (current_location_mcb->size >= numbytes)
				{

					current_location_mcb->is_available = 0;

					memory_location = current_location;

					break;
				}
			}

			current_location = current_location +
							   current_location_mcb->size;
		}

		if (!memory_location)
		{

			sbrk(numbytes);

			memory_location = last_valid_address;

			last_valid_address = last_valid_address + numbytes;

			current_location_mcb = reinterpret_cast<mem_control_block *>(memory_location);
			current_location_mcb->is_available = 0;
			current_location_mcb->size = numbytes;
		}

		memory_location = memory_location + sizeof(struct mem_control_block);

		return memory_location;
	}

	auto init_swap()
	{
		swap_file = open("swap", O_RDWR | O_CREAT | O_TRUNC, 0755);
		lseek(swap_file, 0, SEEK_SET);
		for (int i = 0; i < swap_size; i++)
			write(swap_file, "\0", 1);
		lseek(swap_file, 0, SEEK_SET);
		swap_end = mmap(nullptr, swap_size, PROT_WRITE | PROT_READ, MAP_SHARED, swap_file, 0);
		swap_start = swap_end;
		if (swap_end == invalid_memory)
		{
			return false;
		}
		has_initialized_swap = 1;
		return true;
	}

	void destroy() {
      close(swap_file);
      remove("./swap");
    }

	void* swap(long numbytes){

		void *current_location;
		struct mem_control_block *current_location_mcb;
		void *memory_location;

		if (!has_initialized_swap)
		{
			init_swap();
		}

		numbytes = numbytes + sizeof(struct mem_control_block);
		

		memory_location = 0;

		current_location = swap_start;

		while (current_location != swap_end)
		{

			current_location_mcb = (struct mem_control_block *)current_location;

			if (current_location_mcb->is_available)
			{
				if (current_location_mcb->size >= numbytes)
				{

					current_location_mcb->is_available = 0;

					memory_location = current_location;

					break;
				}
			}

			current_location = current_location +
							   current_location_mcb->size;
		}

		if (!memory_location)
		{

			mmap(nullptr, numbytes, PROT_WRITE | PROT_READ, MAP_SHARED, swap_file, 0);

			memory_location = swap_end;

			swap_end = swap_end + numbytes;

			current_location_mcb = reinterpret_cast<mem_control_block *>(memory_location);
			current_location_mcb->is_available = 0;
			current_location_mcb->size = numbytes;
		}

		memory_location = memory_location + sizeof(struct mem_control_block);

		return memory_location;

	}





}

int main()
{
	int *nums = (int *)mySpace::malloc(sizeof(int) * 5);
	for(int i = 0; i < 5 ; i++){
		nums[i] = i+1;	
		std::cout << nums[i];
	}
	
	std::cout << "\n";

	int *nums_swap = (int*)mySpace::swap(sizeof(int)*2);

	nums_swap[0] = 11;
	nums_swap[1] = 22;

	for(int j =0 ; j <2; j++)
	{
		std::cout << nums_swap[j];
	} 
	std::cout << "\n";
	char *str = (char*)mySpace::swap(sizeof(char)*11);
	strcpy(str,"TESTtest\0");
	printf("%s\n",str);
	
	
	mySpace::free(nums);
	mySpace::free(nums_swap);
	mySpace::free(str);
	mySpace::destroy();
}