/***********************************************************************/
/*************** Copyright (C) 2014-2021 Richard Spinney ***************/
/***********************************************************************/

/*********** This file is distributed WITHOUT ANY WARRANTY. ************/
/******** See the file `LICENSE' for details and other matters. ********/

 
#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

class matrix_2d{

    friend class boost::serialization::access;

protected:
    double m11,m12,m13,m21,m22,m23,m31,m32,m33;

public:
    matrix_2d mult(matrix_2d in) const{
        matrix_2d ret;
        ret.m11 = in.m11 * this->m11 + in.m12 * this->m21 + in.m13 * this->m31;
        ret.m12 = in.m11 * this->m12 + in.m12 * this->m22 + in.m13 * this->m32;
        ret.m13 = in.m11 * this->m13 + in.m12 * this->m23 + in.m13 * this->m33;

        ret.m21 = in.m21 * this->m11 + in.m22 * this->m21 + in.m23 * this->m31;
        ret.m22 = in.m21 * this->m12 + in.m22 * this->m22 + in.m23 * this->m32;
        ret.m23 = in.m21 * this->m13 + in.m22 * this->m23 + in.m23 * this->m33;

        ret.m31 = in.m31 * this->m11 + in.m32 * this->m21 + in.m33 * this->m31;
        ret.m32 = in.m31 * this->m12 + in.m32 * this->m22 + in.m33 * this->m32;
        ret.m33 = in.m31 * this->m13 + in.m32 * this->m23 + in.m33 * this->m33;

        return ret;
    }

    std::vector<double> mult_vec(std::vector<double> in) const{
        double x = m11*in[0] + m12*in[1] + m13;
        double y = m21*in[0] + m22*in[1] + m23;
        return {x,y};
    }

    // debugging
    // void print() const{
    //     std::cout<<m11<<" "<<m12<<" "<<m13<<" "<<m21<<" "<<m22<<" "<<m23<<" "<<m31<<" "<<m32<<" "<<m33<<std::endl;
    // }

    template<class Archive>
    void serialize(Archive & ar, const uint32_t /*version*/)
    {
        ar & m11 & m12 & m13 & m21 & m22 & m23 & m31 & m32 & m33;
        
    }

    matrix_2d(std::vector<std::vector<double> > in):m11(in[0][0]),m12(in[0][1]),m13(in[0][2]),m21(in[1][0]),m22(in[1][1]),m23(in[1][2]),m31(in[2][0]),m32(in[2][1]),m33(in[2][2]){}
    matrix_2d():m11(1.0),m12(0.0),m13(0.0),m21(0.0),m22(1.0),m23(0.0),m31(0.0),m32(0.0),m33(1.0){}
};
BOOST_CLASS_TRACKING(matrix_2d, boost::serialization::track_never)


class translate_matrix : public matrix_2d{
public:
    translate_matrix(double tx,double ty):matrix_2d({{1.0,0.0,tx},{0.0,1.0,ty},{0.0,0.0,1.0}}){}
};

class scale_matrix : public matrix_2d{
public:
    scale_matrix(double sx,double sy):matrix_2d({{sx,0.0,0.0},{0.0,sy,0.0},{0.0,0.0,1.0}}){}
};

class rotate_matrix : public matrix_2d{
public:
    rotate_matrix(double th):matrix_2d({{cos(th),-sin(th),0.0},{sin(th),cos(th),0.0},{0.0,0.0,1.0}}){}
};

#endif /*MATRIX_H*/


