#pragma once

#include <stdexcept>

#include "Geometry.hpp"
#include "KeyPoint.hpp"
#include "StraightLine.hpp"
#include "UnspecifiedLine.hpp"
#include "ArcLine.hpp"
#include "Polyline.hpp"
#include "Plane.hpp"
#include "Area.hpp"
#include "AppParams.hpp"
#include "mathUtils.hpp"

namespace geomlib {
    class GeometryFactory {
    public:
        static const GeometryFactory* getDefaultInstance();
        static void init(AppParams& params);

        KeyPoint* createKeypoint() const;
        KeyPoint* createKeypoint(Point &point) const;

        StraightLine* createStraightLine(KeyPoint* init_point, KeyPoint* final_point) const;
        ArcLine* createArcLine(KeyPoint* init_point, KeyPoint* final_point, Point* mid_point,
            Vector* init_tangent_vector, Vector* final_tangent_vector) const;
        ArcLine* createArcLine(KeyPoint* init_point, KeyPoint* final_point, Plane* plane,
            double radius, Point* center, Point* mid_point) const;

        UnspecifiedLine* createUnspecifiedLine(
            KeyPoint* init_point,
            KeyPoint* final_point,
            Point* mid_point,
            Vector* tangent_init_vector,
            Vector* tangent_final_vector,
            double length) const;

        Polyline* createPolyline(
            KeyPoint* init_point,
            KeyPoint* final_point,
            vector<Line*> lines
        ) const;

        /** 
         * Corrige a quantidade de elementos em uma unidade
         * na linha que apresentar o menor erro na direção
         * especificada.
         * 
         * @param lines Lista de linhas a serem avaliadas
         * @param direction Direções de correção (UP|DOWN|BOTH)
         */
        void lineElementsQtyCorrection(
            vector<Line*> &lines,
            const APROX_DIRECTION direction
            ) const;

        Plane* createPlane(Point* p1, Point* p2, Point* p3) const;
        Plane* createPlane(Point* p1, Vector* v1, Vector* v2) const;

        Area* createArea(vector<Area::Loop*> loops) const;

        GeometryFactory(GeometryFactory const&) = delete;
        void operator=(GeometryFactory const&) = delete;
    private:
        AppParams _appParams;

        static GeometryFactory* getInstanceImpl(AppParams& params);
        GeometryFactory(AppParams& params)
            : _appParams{move(params)}
        {
            if(params.globalElementSize <= 0)
                throw std::runtime_error{ "globalElementSize must be greater than 0" };
        }

        template<typename T>
        T* withElementsQty(T* line) const
        {
            static_assert(std::is_base_of<Line, T>::value, "T not derived from Line");
            int qtdElements = ceil(line->length() / _appParams.globalElementSize);
            line->setElementsQty(qtdElements);
            return line;
        }
    };
}