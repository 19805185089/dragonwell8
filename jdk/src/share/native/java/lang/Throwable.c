/*
 * Copyright (c) 1994, 2011, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

/*
 *      Implementation of class Throwable
 *
 *      former classruntime.c, Wed Jun 26 18:43:20 1991
 */

#include <stdio.h>
#include <signal.h>

#include "jni.h"
#include "jvm.h"

#include "java_lang_Throwable.h"

/*
 * Fill in the current stack trace in this exception.  This is
 * usually called automatically when the exception is created but it
 * may also be called explicitly by the user.  This routine returns
 * `this' so you can write 'throw e.fillInStackTrace();'
 */
JNIEXPORT jobject JNICALL
Java_java_lang_Throwable_fillInStackTrace(JNIEnv *env, jobject throwable, int dummy)
{
    jclass cls = (*env)->GetObjectClass(env, throwable);
    if (cls != NULL) {
        jclass classClass = (*env)->FindClass(env, "java/lang/Class");
        if (classClass != NULL) {
            jmethodID mid = (*env)->GetMethodID(env, classClass, "getName", "()Ljava/lang/String;");
            if (mid != NULL) {
                jstring nameStr = (jstring)(*env)->CallObjectMethod(env, cls, mid);
                if (nameStr != NULL) {
                    const char *className = (*env)->GetStringUTFChars(env, nameStr, NULL);
                    if (className != NULL) {
                        printf("[Crash] Exception class name: %s\n", className);
                        (*env)->ReleaseStringUTFChars(env, nameStr, className);
                    }
                    (*env)->DeleteLocalRef(env, nameStr);
                }
            }
            (*env)->DeleteLocalRef(env, classClass);
        }
        (*env)->DeleteLocalRef(env, cls);
    }
    JVM_FillInStackTrace(env, throwable);
    return throwable;
}

JNIEXPORT jint JNICALL
Java_java_lang_Throwable_getStackTraceDepth(JNIEnv *env, jobject throwable)
{
    return JVM_GetStackTraceDepth(env, throwable);
}

JNIEXPORT jobject JNICALL
Java_java_lang_Throwable_getStackTraceElement(JNIEnv *env,
                                              jobject throwable, jint index)
{
    return JVM_GetStackTraceElement(env, throwable, index);
}
