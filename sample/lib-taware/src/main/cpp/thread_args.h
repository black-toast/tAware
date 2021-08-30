//
// Created by yinhua on 2021/8/30.
//

#ifndef SAMPLE_THREAD_ARGS_H
#define SAMPLE_THREAD_ARGS_H


class thread_args {
public:
    void* (*start_routine)(void*);
    void* start_routine_arg;
};


#endif //SAMPLE_THREAD_ARGS_H
