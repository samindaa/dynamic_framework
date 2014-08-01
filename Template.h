/*
 * Template.h
 *
 *  Created on: Jul 29, 2013
 *      Author: sam
 */

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#include "Framework.h"

// Streams
#define SERIALIZE_1(IN_OUT)         serializeObject(in, out, #IN_OUT, IN_OUT);
#define SERIALIZE_2(NAME, IN_OUT)   serializeObject(in, out, NAME, IN_OUT);

// The interim macro that simply strips the excess and ends up with the required macro
#define SERIALIZE_X(x, NAME, IN_OUT, FUNC, ...)  FUNC

// The macro that the programmer uses
#define SERIALIZE(...)    SERIALIZE_X(,##__VA_ARGS__,                   \
                          SERIALIZE_2(__VA_ARGS__),                     \
                          SERIALIZE_1(__VA_ARGS__)                      \
                            )

// Macros to create the computational units as well as the representations that they provide.
#define MODULE(NAME)                                                        \
class NAME;                                                                 \
class NAME##Base : public ModuleTemplate<NAME>                              \
{                                                                           \
  private: typedef NAME##Base _This;                                        \
  public: NAME##Base() : ModuleTemplate<NAME>() {}                          \
  public: virtual ~NAME##Base() {}                                          \
  public: const char* getName() const { return #NAME ;}                     \
  public: static const char* getNameStatic() { return #NAME ; }             \

#define REQUIRES(REPRESENTATION)                                            \
  private: static const char* getRepresentation##REPRESENTATION() { return #REPRESENTATION ; }       \
  protected: RepresentationRequierer<&_This::getNameStatic, &_This::getRepresentation##REPRESENTATION, REPRESENTATION> the##REPRESENTATION;      \

#define PROVIDES(REPRESENTATION)                                            \
  protected: virtual void update(REPRESENTATION& the##REPRESENTATION) =0;     \
  protected: static void _update##REPRESENTATION(Node* moduleNode, Node* representationNode)         \
          { (((_This*) moduleNode)->update(*((REPRESENTATION*) representationNode))); } \
  protected: RepresentationProvider<&_This::getNameStatic, &_This::_update##REPRESENTATION, REPRESENTATION> _the##REPRESENTATION##Provides;      \

#define USES(REPRESENTATION)                                               \
  private: static const char* getRepresentation##REPRESENTATION() { return #REPRESENTATION ; }       \
  protected: RepresentationUser<&_This::getNameStatic, &_This::getRepresentation##REPRESENTATION, REPRESENTATION> the##REPRESENTATION;                                  \

#define END_MODULE };                                                      \


#define MAKE_MODULE_1(NAME)                               ModuleLoader<NAME> _the##NAME##ModuleTemplate("def", 20);
#define MAKE_MODULE_2(NAME, THREAD_NAME)                  ModuleLoader<NAME> _the##NAME##ModuleTemplate(#THREAD_NAME, 20);
#define MAKE_MODULE_3(NAME, THREAD_NAME, THREAD_PRIORITY) ModuleLoader<NAME> _the##NAME##ModuleTemplate(#THREAD_NAME, THREAD_PRIORITY);

// The interim macro that simply strips the excess and ends up with the required macro
#define MAKE_MODULE_X(x, NAME, THREAD_NAME, THREAD_PRIORITY, FUNC, ...)  FUNC

// The macro that the programmer uses
#define MAKE_MODULE(...)    MAKE_MODULE_X(,##__VA_ARGS__,                   \
                            MAKE_MODULE_3(__VA_ARGS__),                     \
                            MAKE_MODULE_2(__VA_ARGS__),                     \
                            MAKE_MODULE_1(__VA_ARGS__)                      \
                            )

#define REPRESENTATION(NAME)                                                \
class NAME;                                                                 \
class NAME##Base : public RepresentationTemplate<NAME>                      \
{                                                                           \
  private: typedef NAME##Base _This;                                        \
  public: NAME##Base() : RepresentationTemplate<NAME>() {}                  \
  public: virtual ~NAME##Base() {}                                          \
  public: const char* getName()  const { return #NAME ;}                    \
};                                                                          \

#endif /* TEMPLATE_H_ */
