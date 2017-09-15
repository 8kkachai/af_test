#ifndef AF_INTERFACE_H
#define	AF_INTERFACE_H

#ifdef SFLOG_FATAL
#undef SFLOG_FATAL
#endif // SFLOG_FATAL

#ifdef SFLOG_ERROR
#undef SFLOG_ERROR
#endif // SFLOG_ERROR

#ifdef SFLOG_WARN
#undef SFLOG_WARN
#endif // SFLOG_WARN

#ifdef SFLOG_INFO
#undef SFLOG_INFO
#endif // SFLOG_INFO

#ifdef SFLOG_DEBUG
#undef SFLOG_DEBUG
#endif // SFLOG_DEBUG

#ifdef _ECLOG_
#undef _ECLOG_
#endif // !_ECLOG_

#define _ECLOG_(_lev,_FORMAT)                                 		\
{                                                             		\
if (SF_log_level() & _lev)                              \
{                                        \
    char *B;                                    				\
    B = SF_log_get_buffer();              			  	      	\
    (void)sprintf _FORMAT;                                    	\
    ec_log_write_string_poco(_lev, __FILE__, func, __LINE__, B);   	\
}														  		\
}

#define SFLOG_FATAL(_FORMAT)   _ECLOG_(SFLOG_LEVEL_FATAL,_FORMAT)
#define SFLOG_ERROR(_FORMAT)   _ECLOG_(SFLOG_LEVEL_ERROR,_FORMAT)
#define SFLOG_WARN(_FORMAT)    _ECLOG_(SFLOG_LEVEL_WARN,_FORMAT)
#define SFLOG_INFO(_FORMAT)    _ECLOG_(SFLOG_LEVEL_INFO,_FORMAT)
#define SFLOG_DEBUG(_FORMAT)   _ECLOG_(SFLOG_LEVEL_DEBUG,_FORMAT)

#define DO_PRAGMA(x) _Pragma (#x)
#define TODO(x) DO_PRAGMA(message ("TODO - " #x))

TODO(=== THIS IS NEW API FOR NEW EC00 APPLICATION INTERFACE THANK YOU ===)

#endif	/* AF_INTERFACE_H */
