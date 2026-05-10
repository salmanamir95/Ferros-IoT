import { useEffect, useRef } from 'react';
import type { FerrosInsight } from '../types/ferros';
import { InsightCard } from './InsightCard';
import { motion, AnimatePresence } from 'framer-motion';

interface Props {
  events: FerrosInsight[];
}

export const EventFeed: React.FC<Props> = ({ events }) => {
  const containerRef = useRef<HTMLDivElement>(null);

  // Auto-scroll to bottom when new events arrive
  useEffect(() => {
    if (containerRef.current) {
      containerRef.current.scrollTop = containerRef.current.scrollHeight;
    }
  }, [events]);

  return (
    <div className="glass-panel" style={{ display: 'flex', flexDirection: 'column', height: '100%', overflow: 'hidden' }}>
      <div style={{ padding: '1rem', borderBottom: '1px solid var(--border-light)' }}>
        <h2 style={{ fontSize: '1.25rem', fontWeight: 600, display: 'flex', alignItems: 'center', gap: '0.5rem' }}>
          <div style={{ width: '8px', height: '8px', borderRadius: '50%', backgroundColor: 'var(--neon-red)', boxShadow: '0 0 8px var(--neon-red)' }} />
          Live Event Feed
        </h2>
      </div>
      
      <div 
        ref={containerRef}
        className="scroll-hidden"
        style={{ flex: 1, padding: '1rem', overflowY: 'auto', display: 'flex', flexDirection: 'column', gap: '1rem' }}
      >
        <AnimatePresence initial={false}>
          {events.map((event) => (
            <motion.div
              key={`${event.correlation_id}_${event.trace.timestamp_ns}`}
              initial={{ opacity: 0, y: 20 }}
              animate={{ opacity: 1, y: 0 }}
              transition={{ type: 'spring', stiffness: 300, damping: 24 }}
            >
              <InsightCard insight={event} />
            </motion.div>
          ))}
        </AnimatePresence>
        
        {events.length === 0 && (
          <div style={{ textAlign: 'center', color: 'var(--text-muted)', marginTop: '2rem' }}>
            Waiting for telemetry...
          </div>
        )}
      </div>
    </div>
  );
};
