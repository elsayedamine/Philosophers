## 🧠 Summary logic:
- On pthread_create() failure:

- Set table->someone_died = TRUE;
- 
- Loop over 0 to i-1 (created threads) and pthread_join() them;
- 
- Call destroy_table().
- 
- You must not just call destroy_table() immediately because those threads are still using mutexes and memory.
- 
- this while creating the threads 
- if some of the creation fails we do this
- like tell the other threads to stop by telling them that there is a philo died

 