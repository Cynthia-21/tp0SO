package ar.edu.utn.frba.dds.servicioagregador.schedulers;

import ar.edu.utn.frba.dds.servicioagregador.services.IColeccionService;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

@Component
public class ColeccionScheduler {
  private static final Logger logger = LoggerFactory.getLogger(ColeccionScheduler.class);
  private final IColeccionService coleccionService;

  public ColeccionScheduler(IColeccionService coleccionService) {
    this.coleccionService = coleccionService;
  }

  @Scheduled(fixedRate = 3600000) //1 hora
  public void refrescarColeccion() {
    coleccionService.refrescarColecciones()
        .doOnSuccess(v -> logger.info("Refresco de Colecciones exitoso"))
        .doOnError((Throwable e) -> logger.error("Error en el refresco de colecciones: {}", e.getMessage()))
        .subscribe();
  }
}


