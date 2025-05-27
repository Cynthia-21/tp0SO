package domain;

import java.time.LocalDate;
import java.time.Period;

public class Contribuyente {
  private String nombre;
  private String apellido;
  private LocalDate fechaNacimiento;

  public Contribuyente(String nombre, String apellido, Integer edad) {
    this.nombre = nombre;
    this.apellido = apellido;
    this.fechaNacimiento = fechaNacimiento;
  }

  public Integer edad() {
    return Period.between(this.fechaNacimiento, LocalDate.now()).getYears();
  }
}
