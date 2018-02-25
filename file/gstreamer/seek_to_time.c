static void seek_to_time (GstElement *element,
              guint64
              time_ns)
{
    GstEvent *event;
    event = gst_event_new_seek (GST_SEEK_METHOD_SET |
                                GST_FORMAT_TIME,
                                time_ns);
    gst_element_send_event (element, event);
}
