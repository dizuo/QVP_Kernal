#ifndef EXPORT_H
#define EXPORT_H

#pragma warning ( disable: 4251 )
//disable other

#if defined(_MSC_VER)
#  if defined( VP_LIBRARY_STATIC )
#    define VP_EXPORT
#  elif defined( VP_LIBRARY )
#    define VP_EXPORT   __declspec(dllexport)
#  else
#    define VP_EXPORT   __declspec(dllimport)
#  endif
#else
#  define VP_EXPORT
#endif

#endif