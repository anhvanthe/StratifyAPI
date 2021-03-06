#include "sys/Task.hpp"


using namespace sys;


Task::Task(){
    m_id = 0;
}

Task::~Task(){
    m_sys_device.close();
}

void Task::initialize(){
    if( m_sys_device.fileno() < 0 ){
        m_sys_device.open("/dev/sys");
    }
}

int Task::count_total(){
    int idx = m_id;
    int count = 0;
    set_id(0);
    TaskInfo attr;
    while( get_next(attr) >= 0 ){
        count++;
    }
    set_id(idx);
    return count;
}


int Task::count_free(){
    int idx = m_id;
    int count = 0;
    set_id(0);
    TaskInfo attr;
    while( get_next(attr) >= 0) {
        if( !attr.is_enabled() ){
            count++;
        }
    }
    set_id(idx);
    return count;
}


int Task::get_next(TaskInfo & attr){
    sys_taskattr_t task_attr;
    int ret;

    task_attr.tid = m_id;
    m_id++;

    initialize();

    ret = set_error_number_if_error( m_sys_device.ioctl(I_SYS_GETTASK, &task_attr) );
    if( ret < 0 ){
        attr = TaskInfo::invalid();
        return ret;
    }

    attr = task_attr;
    return ret;
}

TaskInfo Task::get_info(int id){
    sys_taskattr_t attr;
    attr.tid = id;
    initialize();
    if( set_error_number_if_error( m_sys_device.ioctl(I_SYS_GETTASK, &attr) ) < 0 ){
        return TaskInfo::invalid();
    }

    return TaskInfo(attr);
}

void Task::print(int pid){
    int count = count_total();
    TaskInfo info;
    TaskInfo::print_header();
    for(int i = 0; i < count; i++){
        info = get_info(i);
        if( (pid < 0 || (pid == (int)info.pid())) && info.is_enabled() ){
            info.print();
        }
    }
}

void TaskInfo::print_header(){
    printf("name(pid,id): prio:value/ceiling mem:total (heap,stack)\n");
}


void TaskInfo::print() const {
    if( is_valid() ){
        if( is_thread() ){
            printf("%s(%ld,%ld): prio:%d/%d memory:%ld (NA,%ld)\n",
                   name(),
                   pid(),
                   id(),
                   priority(), priority_ceiling(),
                   memory_size(), stack_size());
        } else {
            printf("%s(%ld,%ld): prio:%d/%d memory:%ld (%ld,%ld)\n",
                   name(),
                   pid(),
                   id(),
                   priority(), priority_ceiling(),
                   memory_size(),
                   heap_size(), stack_size());
        }
    }
}
