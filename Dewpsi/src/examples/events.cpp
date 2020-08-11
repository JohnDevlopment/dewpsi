//! [Event Dispatcher]
EventDispatcher dispatcher(e);

// window close event
dispatcher.Dispatch<WindowCloseEvent>(PD_BIND_EVENT_FN(Application::OnWindowClosed));
//! [Event Dispatcher]
