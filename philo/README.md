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

➜  aziz git:(main) ✗ ./philo 4 400 200 200 
0 2 has taken a fork
1 2 has taken a fork
1 2 is eating
1 4 has taken a fork
1 1 has taken a fork
201 2 is sleeping
201 3 has taken a fork
201 1 has taken a fork
201 1 is eating
401 1 is sleeping
401 4 has taken a fork
401 4 is eating
401 2 is thinking
401 2 has taken a fork
401 3 died
