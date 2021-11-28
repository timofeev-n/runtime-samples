Интеграция отладчика скриптов (lua) в проект        {#sample_script_debugger_page}
=================

[TOC]

Обзор                     {#debugger_overview}
=====

Отладчик состоит из нескольких компонентов, которые присутствуют в проекте максимально незаметно.
Для работы отладчика внутри приложения запускается контроллер удалённого взаимодействия ([RemoteController](@ref ScriptDebug::SampleRemoteController)), который реализован как сервер, слущающих внешние запросы/команды.
На текущий момент его роль ограничивается обработкой запроса на создание отладочной сессии.

Сессия отладки реализует протокол взаимодействия с Debugger UI посредством [Debug Adapter Protocol](https://microsoft.github.io/debug-adapter-protocol/). Большая часть логики отладочной сессии работает прозрачно для проекта. Минимум что нужно от проектного кода (вещи специфичные для каждого проекта):
* доступ к lua state (внутри библиотека полностью отвязана от движкового lua);
* информация о потоке в котором должен выполняться lua код;

Для реализации этих аспектов, внутри проекта необходимо реализовать функционал класса Lua::Debug::LuaDebugSessionController. См. [пример](@ref ScriptDebug::SampleDebugSession)

Так же проект должен инициализовать подсистему поддержки runtime части: [RuntimeSupplement](@ref App::RuntimeSupplementImpl) которая отвечает:
* базовую поддержку runtime-компонентов и асинхронного выполнения;
* сетевые операции;
* таймеры;

Общие шаги:
* добавление библиотек [lib-cppcommon](https://github.com/Playrix/lib-cppcommon) и [lua-toolkit](https://github.com/Playrix/lua-toolkit) в качестве сабмодулей;
* подключение библиотек внутри проектных [cmake файлов](https://github.com/timofeev-n/runtime-samples/blob/main/CMakeLists.txt);
* инициализация подсистемы поддержки [runtime исполнения](@ref App::RuntimeSupplementImpl);
* реализация [контроллера удалённого взаимодействия](@ref ScriptDebug::SampleRemoteController) и [сессии отладки](@ref ScriptDebug::SampleDebugSession);
* [запуск](@ref ScriptDebug::ScriptDebugSupportImpl) сервиса удалённого взаимодействия: логика инициализации и остановки специфичная для каждого проекта/приложения

Интеграция/запуск поддержки runtime .           {#debugger_runtime_setup}
========


Компоненты отладчика.           {#debugger_debugger_setup}
========


Запуск/остановка сервиса внутри проекта.           {#debugger_inproject_setup}
========