void perf_pretty_print_flags(long int flags);
//static void perf_pretty_print_type(int type);
//static void perf_pretty_print_cache(long long config);
//static void perf_pretty_print_config(long long type, long long config);
//static void perf_pretty_print_sample_type(unsigned long long sample_type);
//static void perf_pretty_print_read_format(unsigned long long read_format);
//static void perf_pretty_print_breakpoint_type(int bp_type);
void perf_pretty_print_attr(struct perf_event_attr *pe, int fd);
void perf_pretty_print_event(int fd,
				struct perf_event_attr *pe,
				pid_t pid, int cpu,
				int group_fd, unsigned long flags);
