#ifndef ASYNCDELTASK_H
#define ASYNCDELTASK_H


#include <QThreadPool>
#include <QRunnable>
#include <QObject>




class AsyncDelTask : public QRunnable
{

    public:

        AsyncDelTask(QObject *object) : QRunnable(), _object(object) {}


    void run()
    {
        if(_object)
            delete _object;
    }


    static void async_del(QObject *object)
    {
        AsyncDelTask *task = new AsyncDelTask(object);

        // QThreadPool takes ownership and deletes 'task' automatically
        QThreadPool::globalInstance()->start(task);
    }


    private:

       QObject *_object;

};






#endif // ASYNCDELTASK_H
