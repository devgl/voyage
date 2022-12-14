//
// Created by GL on 12/14/2022.
//

#ifndef VOYAGE_PCH_H
#define VOYAGE_PCH_H

#ifdef RENDERSYSTEM_IMPL
#define RENDERSYSTEM_API __declspec(dllexport)
#else
#define RENDERSYSTEM_API __declspec(dllimport)
#endif

#endif //VOYAGE_PCH_H
